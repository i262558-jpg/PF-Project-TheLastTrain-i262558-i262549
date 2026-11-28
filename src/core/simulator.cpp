#include "simulator.h"
#include "physics.h"
#include "switches.h"
#include "logger.h"
#include "viewer_iface.h"
#include "util.h"
#include<iostream>
using namespace std;
// Modular simulation loop using primitive variables
bool checkEndConditions(int energy, int time, const bool objectives[NUM_OBJECTIVES])
{
    if(energy<=0 || time<=0)
      return true;
    if(objectives[OBJ_MISSION_WON]||objectives[OBJ_MISSION_FAILED])
       return true;
    return false;
}
bool handleMoveCommand(int targetFloor, int targetRoom,
                       int& currentFloor, int& currentRoom,
                       int& energy, int& time,
                       const char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                       const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                       int& emergencyTripsLeft, bool lift2Operational)
    {
       char connType;
       if(!isDirectlyConnected(currentFloor,currentRoom,targetFloor,targetRoom,horizontalConn,connType))
         return false;
       if (!canTraverse(targetFloor,targetRoom,connType,inventory,inventoryCount,emergencyTripsLeft,lift2Operational))
          return false;
        int energyCost=0;
        int timeCost=0;
        getMoveCost(connType,energyCost,timeCost);
         
     if(connType == '2' && !lift2Operational)
         {
             if(!hasItem(inventory, inventoryCount, ENT_EMERGENCY_CARD))
                  return false;

             if(emergencyTripsLeft <= 0)
               return false;
          }
        
        if(connType=='2'&& !lift2Operational)
           {
               emergencyTripsLeft--;
            }
      applyResourceDelta(energy,time,energyCost,timeCost);
      currentFloor=targetFloor;
      currentRoom=targetRoom;
      return true;
    }

bool handleInspectCommand(int currentFloor, int currentRoom,
                          int& energy, int& time,
                          const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int entityCount,
                          bool objectives[NUM_OBJECTIVES], int& score)
  {
     if(currentFloor<0 || currentFloor>=NUM_FLOORS || currentRoom<0 || currentRoom>=ROOMS_PER_FLOOR)
       {
        return false;
       }
    if(entityCount<0 || entityCount>MAX_ENTITIES_PER_ROOM)
       {
        return false;
       }
    if(energy<=0 || time<=0)
     {
      return false;
     }
    if(entityCount==0)
       {
        cout<<"This room is Empty"<<endl;
       } 
    else
       {
         cout<<"This room is not empty. It has entities"<<endl;
    
          for(int i=0;i<entityCount;i++)
            {
              char entity=roomEntities[currentFloor][currentRoom][i];
              cout<<entity<<endl;
              if (entity == ENT_CLUE_4 && !objectives[OBJ_CLUE_4])
                {
                  objectives[OBJ_CLUE_4]=true;
                  applyScoreDelta(score,SCORE_CLUE_FOUND);
                  cout<<"Clue 4 discovered."<<endl;
                }
              else if(entity==ENT_CLUE_2&&!objectives[OBJ_CLUE_2])
                {
                  objectives[OBJ_CLUE_2]=true;
                  applyScoreDelta(score,SCORE_CLUE_FOUND);
                  cout<<"Clue 2 discovered."<<endl;
                }
              else if(entity==ENT_CLUE_7&&!objectives[OBJ_CLUE_7])
                {
                   objectives[OBJ_CLUE_7]=true;
                   applyScoreDelta(score,SCORE_CLUE_FOUND);
                   cout<<"Clue 7 discovered."<<endl;
                }
           }
        }
    applyResourceDelta(energy,time,COST_INSPECT_ENERGY,COST_INSPECT_TIME);
    return true;
  }
bool handleCollectCommand(char itemSymbol, int currentFloor, int currentRoom,
                          int& energy, int& time, int& score,
                          char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                          int& entityCount,
                          char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                          bool objectives[NUM_OBJECTIVES],int& emergencyTripsLeft)
  {
     if(currentFloor<0 || currentFloor>=NUM_FLOORS || currentRoom<0 || currentRoom>=ROOMS_PER_FLOOR)
       {
        return false;
       }
    if(entityCount<0 || entityCount>MAX_ENTITIES_PER_ROOM)
       {
        return false;
       }
    if(energy<=0 || time<=0)
     {
      return false;
     }
     int index=-1;
     for(int i=0;i<entityCount;i++)
       {
        if(roomEntities[currentFloor][currentRoom][i]==itemSymbol)
         {
          index=i;
          break;
         }
       }  
      if(index==-1)
      {
        cout<<"No item in this room"<<endl;
        return false;
      }
    bool requiresInventory=true;
    if (itemSymbol==ENT_ENERGY_PACK||itemSymbol==ENT_EMERGENCY_TOOLKIT||itemSymbol == ENT_OBSERVATION_TOKEN)
    {
        requiresInventory=false;
    }
    else if(itemSymbol !=ENT_MASTER_KEY&&itemSymbol!=ENT_FUSE&&itemSymbol!=ENT_LIFT_PASS&& itemSymbol!=ENT_MEDICAL_KIT&&itemSymbol!=ENT_EMERGENCY_CARD&&itemSymbol!=ENT_CAT_CARRIER && itemSymbol!=ENT_CABINET_TRAIN_KEY)
    {
        cout<<"entity cannot be collected."<<endl;
        return false;
    }
    if(requiresInventory&&inventoryCount>=MAX_INVENTORY_CAPACITY)
    {
      cout<<"inventory is full."<<endl;
      return false;
    }
    if(requiresInventory)
    {
      if(!addItem(inventory,inventoryCount,itemSymbol))
        {
          return false;
        }
    }
    if(itemSymbol==ENT_ENERGY_PACK)
    {
      applyResourceDelta(energy,time,BONUS_ENERGY_PACK,0);
    }
    else if(itemSymbol==ENT_EMERGENCY_TOOLKIT)
    {
      applyResourceDelta(energy,time,BONUS_EMERGENCY_TOOLKIT,0);
    }
    else if(itemSymbol==ENT_OBSERVATION_TOKEN)
    {
      applyResourceDelta(energy,time,0,BONUS_OBSERVATION_TOKEN);
    }
    else if(itemSymbol==ENT_MASTER_KEY)
    {
       if(!objectives[OBJ_MASTER_KEY])
        {
          objectives[OBJ_MASTER_KEY]=true;
          applyScoreDelta(score,SCORE_KEY_COLLECT);
        }
    }
    else if(itemSymbol==ENT_FUSE)
    {
      objectives[OBJ_FUSE_COLLECTED]=true;
    }
    else if(itemSymbol==ENT_LIFT_PASS)
    {
      if(!objectives[OBJ_LIFT_PASS])
        {
          objectives[OBJ_LIFT_PASS]=true;
          applyScoreDelta(score,SCORE_LIFT_PASS);
        }
    }
    else if(itemSymbol==ENT_CAT_CARRIER)
    {
      objectives[OBJ_CAT_CARRIER]=true;
    }
    else if(itemSymbol == ENT_EMERGENCY_CARD)
      {
          emergencyTripsLeft = 3;
        }
    else if(itemSymbol==ENT_CABINET_TRAIN_KEY)
       {
          if(!objectives[OBJ_TRAIN_KEY])
            {
             objectives[OBJ_TRAIN_KEY]=true;
              applyScoreDelta(score,SCORE_TRAIN_KEY);
            }
        }
    applyResourceDelta(energy,time,COST_ITEM_PICKUP_ENERGY,COST_ITEM_PICKUP_TIME);
    for (int i=index;i<entityCount-1;i++)
    {
      roomEntities[currentFloor][currentRoom][i]=roomEntities[currentFloor][currentRoom][i + 1];
    }
  entityCount--;
  roomEntities[currentFloor][currentRoom][entityCount]=ENT_NONE;
  cout<<"Item collected successfully."<<endl;
  return true;
}
 bool handleUseCommand(char itemSymbol, int currentFloor, int currentRoom,
                      int& energy, int& time, int& score,
                      char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                      bool objectives[NUM_OBJECTIVES],char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1])
  {
     if(currentFloor<0 || currentFloor>NUM_FLOORS-1 || currentRoom<0 || currentRoom>ROOMS_PER_FLOOR-1)
     {
       return false;
     }
     if(!hasItem(inventory,inventoryCount,itemSymbol))
     {
      return false;
     }
    if(itemSymbol==ENT_FUSE &&(currentFloor!=2|| currentRoom!=3|| objectives[OBJ_FUSE_INSTALLED]))
    {
      return false;
    }
    
    if(itemSymbol==ENT_MEDICAL_KIT &&(currentFloor!=3|| currentRoom!=0|| objectives[OBJ_PASSENGER_RESCUED]))
    {
      return false;
    }
    if(itemSymbol==ENT_CAT_CARRIER &&(currentFloor!=2|| currentRoom!=2|| objectives[OBJ_CAT_SECURED]))
    {
      return false;
    }
    if(itemSymbol==ENT_MASTER_KEY&&(currentFloor!=2|| (currentRoom!=0 && currentRoom!=1)))
    {
      return false;
    }
   if(itemSymbol== ENT_MASTER_KEY&&horizontalConn[2][0]!=CONN_LOCKED_DOOR)
     {
       return false;
     } 
    if(itemSymbol==ENT_FUSE)
    {
       if(!installFuseControlPanel(inventory,inventoryCount,objectives,score))
       {
          return false;      
      }
    }
    else if(itemSymbol==ENT_MEDICAL_KIT)
       {
        objectives[OBJ_PASSENGER_RESCUED]=true;
        applyScoreDelta(score,SCORE_RESCUE_PASSENGER);
        removeItem(inventory,inventoryCount,ENT_MEDICAL_KIT);
       }
     else if(itemSymbol==ENT_CAT_CARRIER)
       {
        objectives[OBJ_CAT_SECURED]=true;
        applyScoreDelta(score,SCORE_SECURE_CAT);
        removeItem(inventory,inventoryCount,ENT_CAT_CARRIER);
       }
      else if(itemSymbol==ENT_MASTER_KEY)
        {
          if(!unlockFloor3Door(inventory,inventoryCount))
            {
              return false;
            }
          horizontalConn[2][0] = CONN_DOOR;
        }
    else
    {
      return false;
    }
    applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
    return true;
  }
bool handleUnlockCabinetCommand(const char* code, int currentFloor, int currentRoom,
                                bool objectives[NUM_OBJECTIVES], int& score,
                                char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                                int& entityCount, int& energy, int& time)
  {
    if(currentFloor!=4 || currentRoom!=0)
    {
      return false;
    }
    if(objectives[OBJ_CABINET_UNLOCKED])
    {
      return false;
    }
    if(code==nullptr || !verifyCabinetCode(code,objectives))
    {
      return false;
    }
    if(entityCount<0 || entityCount>=MAX_ENTITIES_PER_ROOM)
    {
      return false;
    }
    for(int i=0;i<entityCount;i++)
    {
      if(roomEntities[4][0][i]==ENT_CABINET_TRAIN_KEY)
      {
        return false;
      }
    }
    roomEntities[4][0][entityCount]=ENT_CABINET_TRAIN_KEY;
    entityCount++;
    objectives[OBJ_CABINET_UNLOCKED]=true;
    applyScoreDelta(score,SCORE_CABINET_UNLOCK);
    applyResourceDelta(energy,time,COST_ITEM_USE_ENERGY,COST_ITEM_USE_TIME);
    return true;
  }
void runSimulation(int& playerFloor, int& playerRoom,
                   int& energy, int& time, int& score,
                   char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                   bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                   char horizontalConn[NUM_FLOORS][ROOMS_PER_FLOOR - 1],
                   char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                   int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{
    char command[20];
    bool running=true;

    while(running)
    {
      if(energy<=0||time<=0)
        {
            objectives[OBJ_MISSION_FAILED]=true;
            cout << "\nMISSION FAILED! Energy or Time depleted.\n";
            break;
        }

      objectives[OBJ_LIFT2_ACTIVE]=objectives[OBJ_FUSE_INSTALLED]&&objectives[OBJ_LIFT_PASS];
      bool lift2Operational=objectives[OBJ_LIFT2_ACTIVE];
       cout << "\n====================================\n";
       cout << "          THE LAST TRAIN            \n";
       cout << "====================================\n";

   renderDashboard(playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,emergencyTripsLeft);
   renderRoomDetails(playerFloor,playerRoom,roomEntities,entityCounts[playerFloor][playerRoom]);
    renderObjectivesChecklist(objectives);

cout<<"\nCommands:\n";
cout<<"move,inspect,collect,use,unlock\n";
cout<<"board,save,load,map,status,quit\n";
cout<<"\nEnter command:";
cin.width(20);
 cin>>command;

if(!cin)
{
  cin.clear();
  cin.ignore(1000, '\n');
  cout<<"Invalid input.\n";
  continue;
 }

    if(stringEquals(command,"move"))
    {
      int targetFloor,targetRoom;
        cout<<"Enter target floor (1-5):";
        cin>>targetFloor;
        cout<<"Enter target room (1-4):";
        cin>>targetRoom;

      if(!cin)
      {
        cin.clear();
        cin.ignore(1000,'\n');
        cout<<"Invalid coordinates.\n";
        continue;
      }

       targetFloor--;
        targetRoom--;
        if(handleMoveCommand(targetFloor,targetRoom,playerFloor,playerRoom,energy,time,horizontalConn,inventory,inventoryCount,emergencyTripsLeft,lift2Operational))
          {
          cout<<"Movement successful!\n";
          }
        else
         {
            cout<<"Movement failed. Check connection or access.\n";
         }
  }
else if(stringEquals(command,"inspect"))
  {
    renderRoomDetails(playerFloor,playerRoom,roomEntities,entityCounts[playerFloor][playerRoom]);
    if(!handleInspectCommand(playerFloor,playerRoom,energy,time,roomEntities,entityCounts[playerFloor][playerRoom],objectives,score))
        {
        cout<<"Inspection failed.\n";
        }
  }
else if(stringEquals(command,"collect"))
  {
    char itemSymbol;
    cout<<"Enter item symbol:";
    cin>>itemSymbol;
    if(!handleCollectCommand(itemSymbol,playerFloor,playerRoom,energy,time,score,roomEntities,entityCounts[playerFloor][playerRoom],inventory,inventoryCount,objectives,emergencyTripsLeft))
      {
        cout<<"Collection failed."<<endl;
      }
  }
else if(stringEquals(command,"use"))
    {
       char itemSymbol;
      cout<<"Enter item symbol to use:";
      cin>>itemSymbol;
      if(!handleUseCommand(itemSymbol,playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,horizontalConn))
       {
        cout<<"Cannot use this item here."<<endl;
        }
    }
else if(stringEquals(command,"unlock"))
   {
    char code[8];
    cout<<"Enter cabinet code:";
    cin.width(8);
    cin>>code;
    if(handleUnlockCabinetCommand(code,playerFloor,playerRoom,objectives,score,roomEntities,entityCounts[4][0],energy,time))
      {
        cout<<"Cabinet unlocked! Train Key is available."<<endl;
      }
    else
     {
       cout<<"Incorrect code or cabinet cannot be unlocked here."<<endl;
     }
  }
else if(stringEquals(command,"board"))
  {
    if(playerFloor==4&&playerRoom==1&&objectives[OBJ_TRAIN_KEY]&&objectives[OBJ_PASSENGER_RESCUED]&&objectives[OBJ_CAT_SECURED]&&objectives[OBJ_LIFT2_ACTIVE]&&!objectives[OBJ_TRAIN_PREPARED])
      {
        objectives[OBJ_TRAIN_PREPARED]=true;
        applyScoreDelta(score,SCORE_PREPARE_TRAIN);
        objectives[OBJ_MISSION_WON]=true;
        applyScoreDelta(score,SCORE_MISSION_VICTORY);
        cout<<"\nTRAIN BOARDED! MISSION COMPLETED!\n";
        running=false;
       }
    else
      {
        cout<<"Train is not ready. Complete the required objectives"<<"and reach Floor 5, Room 2"<<endl;
            }
  }
else if(stringEquals(command,"save"))
  {
    if(saveGameStateToFile("out/savegame.txt",playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,roomEntities,entityCounts))
       {
        cout<<"Game saved successfully."<<endl;
        }
    else
      {
        cout<<"Could not save game."<<endl;
      }
  }
else if(stringEquals(command,"load"))
  {
    if(loadGameStateFromFile("out/savegame.txt",playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,objectives,emergencyTripsLeft,roomEntities,entityCounts))
      {
        cout<<"Game loaded successfully."<<endl;
      }
    else
      {
        cout<<"Could not load saved game."<<endl;
      }
  }
else if(stringEquals(command,"map"))
{
   renderAsciiBuilding(playerFloor,playerRoom);
  }
 else if(stringEquals(command,"status"))
  {
    renderDashboard(playerFloor,playerRoom,energy,time,score,inventory,inventoryCount,emergencyTripsLeft);
     renderObjectivesChecklist(objectives);
  }
else if(stringEquals(command,"quit"))
  {
    cout<<"Exiting simulation..."<<endl;
    running = false;
  }
 else
  {
    cout<<"Unknown command. Try again."<<endl;
  }

  if(energy<=0||time<=0)
    {
      objectives[OBJ_MISSION_FAILED]=true;
      cout<<"\nMISSION FAILED! Energy or Time depleted.\n";
      running=false;
    }
  if(checkEndConditions(energy,time,objectives))
    {
      running=false;
    }
  }
bool victory=objectives[OBJ_MISSION_WON];
writeGameSummaryReport("out/game_summary.txt",score,energy,time,objectives,victory);
  cout<<"\n====================================\n";
  cout<<"          SIMULATION ENDED          \n";
  cout<<"====================================\n";
  cout<<"Final Score:"<<score<<endl;
  cout<<"Remaining Energy:"<<energy<<endl;
  cout<<"Remaining Time:"<<time<<endl;
  cout<<"Summary saved to out/game_summary.txt\n";
}


     
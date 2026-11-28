#include "logger.h"
#include "util.h"
#include<fstream>
using namespace std;
bool saveGameStateToFile(const char* filename,
                         int playerFloor, int playerRoom,
                         int energy, int time, int score,
                         const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount,
                         const bool objectives[NUM_OBJECTIVES], int emergencyTripsLeft,
                         const char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                         const int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
{

  ofstream fout(filename);
    if(!fout)
      return false;
    else
     {
      fout<<playerFloor<<'\n';
      fout<<playerRoom<<'\n';
      fout<<energy<<'\n';
      fout<<time<<'\n';
      fout<<score<<'\n';
      fout<<inventoryCount<<'\n';
      fout<<emergencyTripsLeft<<'\n';
    
      for(int i=0;i<inventoryCount;i++)
           {
            fout<<inventory[i]<<'\n';
        }
      for(int i=0;i<NUM_FLOORS;i++)
        {
            for(int j=0; j<ROOMS_PER_FLOOR;j++)
           {
            for(int k=0;k<MAX_ENTITIES_PER_ROOM;k++)
             {
            fout<<static_cast<int>(roomEntities[i][j][k])<<'\n';
           
             }
            }
        }  
      for(int i=0; i<NUM_FLOORS;i++)
          {
            for(int j=0; j<ROOMS_PER_FLOOR;j++)
              {
                fout<<entityCounts[i][j]<<'\n';
              }
         }
      for(int i=0;i<NUM_OBJECTIVES;i++)
          {
            fout<<objectives[i]<<'\n';
          } 
        fout.close(); 
        if (!fout)
            return false;
       return true;
        }
}
bool loadGameStateFromFile(const char* filename,
                           int& playerFloor, int& playerRoom,
                           int& energy, int& time, int& score,
                           char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                           bool objectives[NUM_OBJECTIVES], int& emergencyTripsLeft,
                           char roomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM],
                           int entityCounts[NUM_FLOORS][ROOMS_PER_FLOOR])
 {
    ifstream fin(filename);

    if(!fin)
        return false;

    int tempFloor, tempRoom, tempEnergy, tempTime, tempScore;
    int tempInventoryCount, tempEmergencyTrips;

    char tempInventory[MAX_INVENTORY_CAPACITY];
    bool tempObjectives[NUM_OBJECTIVES];

    char tempRoomEntities[NUM_FLOORS][ROOMS_PER_FLOOR][MAX_ENTITIES_PER_ROOM];
    int tempEntityCounts[NUM_FLOORS][ROOMS_PER_FLOOR];

    fin>>tempFloor;
    fin>>tempRoom;
    fin>>tempEnergy;
    fin>>tempTime;
    fin>>tempScore;
    fin>>tempInventoryCount;
    fin>>tempEmergencyTrips;

    if(!fin)
      return false;

    if(tempFloor<0||tempFloor>=NUM_FLOORS)
        return false;

    if(tempRoom<0||tempRoom>=ROOMS_PER_FLOOR)
        return false;

    if(tempEnergy<0||tempEnergy>MAX_ENERGY)
        return false;

    if(tempTime<0||tempTime>MAX_TIME)
        return false;

    if(tempScore<0||tempScore>520)
        return false;

    if(tempInventoryCount<0||tempInventoryCount>MAX_INVENTORY_CAPACITY)
        return false;

    if(tempEmergencyTrips<0||tempEmergencyTrips>3)
        return false;

    for(int i=0;i<tempInventoryCount;i++)
    {
        fin>>tempInventory[i];
        if(!fin)
            return false;
         char item=tempInventory[i];
        if(item !=ENT_MASTER_KEY&&item!=ENT_FUSE&&item!=ENT_LIFT_PASS&&item!=ENT_MEDICAL_KIT&&item!=ENT_EMERGENCY_CARD&&item!=ENT_CAT_CARRIER&&item!=ENT_CABINET_TRAIN_KEY)
            return false;
    }
    for(int i=0;i< NUM_FLOORS;i++)
    {
        for(int j=0;j<ROOMS_PER_FLOOR;j++)
        {
          for(int k=0;k<MAX_ENTITIES_PER_ROOM;k++)
            {
              int entityCode;
              fin>>entityCode;
                if(!fin||entityCode<0||entityCode>127)
                    return false;
                tempRoomEntities[i][j][k]=static_cast<char>(entityCode);
            }
        }
    }
    for(int i=0;i<NUM_FLOORS;i++)
      {
        for(int j=0;j<ROOMS_PER_FLOOR;j++)
         {
            fin>>tempEntityCounts[i][j];
            if(!fin)
                return false;
            if(tempEntityCounts[i][j]<0||tempEntityCounts[i][j]>MAX_ENTITIES_PER_ROOM)
                return false;
         }
    }
    for(int i=0;i<NUM_OBJECTIVES;i++)
     {
        fin>>tempObjectives[i];
        if(!fin)
            return false;
     }
    playerFloor=tempFloor;
    playerRoom=tempRoom;
    energy=tempEnergy;
    time=tempTime;
    score=tempScore;
    inventoryCount=tempInventoryCount;
    emergencyTripsLeft=tempEmergencyTrips;
    for(int i=0;i<tempInventoryCount;i++)
     {   inventory[i]=tempInventory[i];
    }
    for(int i=0;i<NUM_FLOORS;i++)
    {
        for(int j=0;j<ROOMS_PER_FLOOR;j++)
        {
          entityCounts[i][j]=tempEntityCounts[i][j];
            for(int k=0;k<MAX_ENTITIES_PER_ROOM;k++)
                {
                  roomEntities[i][j][k]=tempRoomEntities[i][j][k];
                }
        }
    }
    for(int i=0;i<NUM_OBJECTIVES;i++)
       {
        objectives[i]=tempObjectives[i];
       }
    fin.close();
    return true;
}
  

void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory,
                            const char sessionAudit[][50], int auditCount)
{
    ofstream fout(filename);

    if(!fout)
        return;

    fout<<"Final Score:"<<score<<'\n';
    fout<<"Remaining Energy:"<<remainingEnergy<<'\n';
    fout<<"Remaining Time:"<<remainingTime<<'\n';

    if(victory)
        fout<<"Mission Status: Victory"<<'\n';
    else
       { fout<<"Mission Status:Failed"<<'\n';}
    const char* names[NUM_OBJECTIVES] = {
        "Master Key collected",
        "Clue digit 4 found",
        "Clue digit 2 found",
        "Clue digit 7 found",
        "Cabinet unlocked",
        "Train Key acquired",
        "Fuse collected",
        "Fuse installed",
        "Lift Pass collected",
        "Lift 2 active",
        "Passenger rescued",
        "Cat Carrier collected",
        "Cat secured",
        "Train prepared",
        "Mission won",
        "Mission failed"
    };

    fout<<"\nOBJECTIVE CHECKLIST\n";

    for(int i=0;i<NUM_OBJECTIVES;i++)
    {
        fout<<names[i]<<":";

        if(objectives[i])
            fout<<"Completed\n";
        else
            fout<<"Incomplete\n";
    }

    fout<<"\nSESSION AUDIT\n";

    for(int i=0;i<auditCount;i++)
    {
        fout<<i + 1<<"."<<sessionAudit[i]<<'\n';
    }

    fout.close();
}
                       
// File persistence for savegame.txt and game_summary.txt using primitive variables

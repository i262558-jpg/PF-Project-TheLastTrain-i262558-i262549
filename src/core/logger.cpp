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
     else
      {
       fin>>playerFloor;
       fin>>playerRoom;
       fin>>energy;
       fin>>time;
       fin>>score;
       fin>>inventoryCount;
       fin>>emergencyTripsLeft;
      if (inventoryCount<0||inventoryCount>MAX_INVENTORY_CAPACITY)
           return false;
      for(int i=0;i<inventoryCount;i++)
           {
            fin>>inventory[i];
        }
      for(int i=0;i<NUM_FLOORS;i++)
        {
            for(int j=0; j<ROOMS_PER_FLOOR;j++)
           {
            for(int k=0;k<MAX_ENTITIES_PER_ROOM;k++)
             {
                int entityCode;
                fin>>entityCode;
                roomEntities[i][j][k]=static_cast<char>(entityCode);
           
             }
            }
        }  
      for(int i=0; i<NUM_FLOORS;i++)
          {
            for(int j=0; j<ROOMS_PER_FLOOR;j++)
              {
                fin>>entityCounts[i][j];
              }
         }
      for(int i=0;i<NUM_OBJECTIVES;i++)
          {
            fin>>objectives[i];
          } 
         
        if (!fin)
           return false;

       if (playerFloor<0||playerFloor>=NUM_FLOORS)
           return false;

       if (playerRoom<0||playerRoom>=ROOMS_PER_FLOOR)
            return false;

       if (energy<0||energy>MAX_ENERGY)
            return false;

       if (time<0||time>MAX_TIME)
            return false;

        for (int i=0;i<NUM_FLOORS;i++)
            {
             for (int j = 0; j < ROOMS_PER_FLOOR; j++)
              {
                if (entityCounts[i][j]<0||entityCounts[i][j]>MAX_ENTITIES_PER_ROOM)
                    return false;
              }
            }

       fin.close();
    return true;
        
        }
}
void writeGameSummaryReport(const char* filename,
                            int score, int remainingEnergy, int remainingTime,
                            const bool objectives[NUM_OBJECTIVES], bool victory)
  {
    ofstream fout(filename);
     if(!fout)
      return;
     else
      {
        fout<<"Final Score:"<<score<<'\n';
      fout<<"Remaining Energy:"<<remainingEnergy<<'\n';
      fout<<"Remaining Time:"<<remainingTime<<'\n';
      if(victory)
        fout<<"Mission Status:Victory"<<'\n';
      else
        fout<<"Mission Status:Failed"<<'\n';
      for(int i=0;i<NUM_OBJECTIVES;i++)
        {
            fout<<"Objective"<<i+1<<":";
            if(objectives[i])
              fout<<"Completed"<<'\n';
            else
               fout<<"Incomplete"<<'\n';
        }
    }
    if(!fout)
     return;
    fout.close();
  }                          
      
                       
// File persistence for savegame.txt and game_summary.txt using primitive variables

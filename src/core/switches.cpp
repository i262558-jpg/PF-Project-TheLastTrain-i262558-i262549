#include "switches.h"
#include "util.h"
#include "physics.h"
#include<cstring>

// Door lock mechanics, lifts, and cabinet verification without structures
bool canUseLift1(int fromFloor, int toFloor)
{
  if(fromFloor==0 && toFloor==1)
    return true;
  else if(fromFloor==1 && toFloor==0)
    return true;
 else
   return false;
}
bool canUseLift2(int fromFloor, int toFloor, bool lift2Operational, int emergencyTripsLeft)
{
    if((fromFloor==1&&toFloor==3 )||( fromFloor==3 && toFloor==1)||(fromFloor==1 && toFloor==4 )|| (fromFloor==4 && toFloor==1)||(fromFloor==3 && toFloor==4) ||( fromFloor==4 && toFloor==3))
    return false;
   
    if (lift2Operational)
      return true;
    
    if(emergencyTripsLeft>0)
      return true;
  return false;
} 
bool unlockFloor3Door(const char inventory[MAX_INVENTORY_CAPACITY], int inventoryCount)
{
   for(int i=0; i<inventoryCount;i++)
    {
        if(inventory[i]==ENT_MASTER_KEY)
         return true;
    }
  return false;    
}
bool installFuseControlPanel(char inventory[MAX_INVENTORY_CAPACITY], int& inventoryCount,
                             bool objectives[NUM_OBJECTIVES], int& score)
    {

        for(int i=0;i<inventoryCount;i++)
          {
            if(inventory[i]==ENT_FUSE)
             {
                removeItem(inventory,inventoryCount,ENT_FUSE);
              objectives[OBJ_FUSE_INSTALLED]=true;
              score+=SCORE_INSTALL_FUSE;
              return true;
             }
          }    
        return false;  
            }
bool verifyCabinetCode(const char* enteredCode, const bool objectives[NUM_OBJECTIVES])
{
   if(strcmp(enteredCode,"427")!=0)
      return false;
   if(!objectives[OBJ_CLUE_4]||!objectives[OBJ_CLUE_2]||!objectives[OBJ_CLUE_7])
        return false;
    return true;
}
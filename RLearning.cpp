
#include <stdio.h>
#include "RLearning.h"




/******************************************************   You need to implement the two functions below   ***********************************************/
float RLearning::f(Point point, int action)
{
	return states[point.y][point.x].q_values[action] + K / states[point.y][point.x].N[action];
}
/*return the biggest value for each state*/
float RLearning::QvalueMax(float q[4])
{
	float value = -10000;
	for (int i = 0; i < 4; i++)
	{
		if (q[i]>value)
		{
			value = q[i];
		}
	}
	return value;
}

/* This function should return one of the four actions from "GO_EAST", "GO_WEST", "GO_NORTH", and "GO_SOURTH". The action should be generated based on the "COUNT" and "Q-values" for each of the four actions at the current state. Then choose the one with the maximum f() value: f(Q, N) = Q + K/N, where Q represents the current q value for action a; N is the count recorded for taking action a from current state. K is just a constant value. You can create any variables (or data members) as you need in this class (either in the "RLearning.h" file or "RLearning.cpp" file */
int RLearning::getAction()
{
	float q[4];
	q[3] = f(cur_loc, 3);
	q[2] = f(cur_loc, 2);
	q[1] = f(cur_loc, 1);
	q[0] = f(cur_loc, 0);
	float max = -100;
    int action ; //this is just an example, you need to change the action value based on the exploration function
	
	for (int i = 0; i < 4; i++)
	{
		if (q[i] >max)
		{
			max = q[i];
			action = i;
			
		}
	}
    return action;
}
/*compute the f function for each Q value*/

/*Actually this function is suggested to implemented first. This function is similar to the function you implemented as last assignment for MDP. According to the input action and the current location "cur_loc", determine the next location of the variable. To make it easier, this time we assume if you take action north, you have 100% to land to the north as long as there is another square available in the north. Only if you are already on the North bounary or the square in the north is a wall, which is at the location (1, 1), you will bounce back to current location. After the next location is determined, you need to update the corresponding q value. Finally update the variable "cur_loc" by the new location coordinate. There are three steps involved in this function:*/
void RLearning::move(int action)
{

	/*store new Q value and old Q value after computation*/
	float q1 = 0.0;
	float q2 = 0.0;
	/*store the current row and col value*/
	int row, col;
	row = cur_loc.y;
	col = cur_loc.x;
	/*store four directions' Q value*/
	float q[4];
	q[0] = f(cur_loc, 0);
	q[1] = f(cur_loc, 1);
	q[2] = f(cur_loc, 2);
	q[3] = f(cur_loc, 3);
	/*add N value*/
	states[row][col].N[action]++;
	if (action == GO_EAST)
	{
		if ((row == 1) && (col == 0))
		{
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*(QvalueMax(q));
			q2 = (1 - ALPHA)*states[row][col].q_values[0] + ALPHA*q1;
		}
		else if (col != 3)
		{
			if ((row == 0) && (col == 2))
			{
				/*compute old Q value and new Q value*/
				q2 = (1 - ALPHA)*states[row][col].q_values[0] + ALPHA;
				cur_loc.x++;
			}
			else if ((row == 1) && (col == 2))
			{
				/*compute old Q value and new Q value*/
				q2 = (1 - ALPHA)*states[row][col].q_values[0] - ALPHA;
				cur_loc.x++;
			}
			else
			{
				cur_loc.x++;
				q[0] = f(cur_loc, 0);
				q[1] = f(cur_loc, 1);
				q[2] = f(cur_loc, 2);
				q[3] = f(cur_loc, 3);
				/*compute old Q value and new Q value*/
				q1 = REWARD + GAMMA*(QvalueMax(q));
				q2 = (1 - ALPHA)*states[row][col].q_values[0] + ALPHA*q1;
			}
		}
		else
		{
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[0] + ALPHA*q1;
		}
	}
	else if (action == GO_SOUTH)
	{

		if ((row == 0) && (col == 1))
		{
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[1] + ALPHA*q1;
		}
		else if (row != 2)
		{
			cur_loc.y++;
			q[0] = f(cur_loc, 0);
			q[1] = f(cur_loc, 1);
			q[2] = f(cur_loc, 2);
			q[3] = f(cur_loc, 3);
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[1] + ALPHA*q1;
		}
		else
		{
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[1] + ALPHA*q1;
		}
	}
	else if (action == GO_WEST)
	{
		if ((col == 2) && (row == 1))
		{
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[2] + ALPHA*q1;
		}
		else if (col != 0)
		{
			cur_loc.x--;
			q[0] = f(cur_loc, 0);
			q[1] = f(cur_loc, 1);
			q[2] = f(cur_loc, 2);
			q[3] = f(cur_loc, 3);
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[2] + ALPHA*q1;	
		}
		else
		{
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[2] + ALPHA*q1;
		}
	}
	
	else
	{
		if ((col == 1) && (row == 2))
		{
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[3] + ALPHA*q1;
		}
		else if (row != 0)
		{
			if ((row == 2) && (col == 3))
			{
				/*compute old Q value and new Q value*/
				q2 = (1 - ALPHA)*states[row][col].q_values[0]- ALPHA;
				cur_loc.y--;
			}
			else 
			{
				cur_loc.y--;
				q[0] = f(cur_loc, 0);
				q[1] = f(cur_loc, 1);
				q[2] = f(cur_loc, 2);
				q[3] = f(cur_loc, 3);
				/*compute old Q value and new Q value*/
				q1 = REWARD + GAMMA*QvalueMax(q);
				q2 = (1 - ALPHA)*states[row][col].q_values[3] + ALPHA*q1;
			}
		}
		else
		{
			/*compute old Q value and new Q value*/
			q1 = REWARD + GAMMA*QvalueMax(q);
			q2 = (1 - ALPHA)*states[row][col].q_values[3] + ALPHA*q1;
		}
	} 
	/*renew the current location's Q value*/
	states[row][col].q_values[action] = q2;
	
    
}

/********************************************************   End of your assignment for this file    *************************************************************/


RLearning::RLearning()
{
    /*Initialize all the state with 0.0 state_value and 0.0 Q_values*/
    for(int y = 0; y < 3; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            states[y][x].location.x = x; //specify the location for this state
            states[y][x].location.y = y;
            
            states[y][x].state_value = 0.0; //define the state value
            states[y][x].q_values[0] = 0.0; //define the Q value
            states[y][x].q_values[1] = 0.0;
            states[y][x].q_values[2] = 0.0;
            states[y][x].q_values[3] = 0.0;
            
        }
    }
    
    /* Set the special Q value for (0, 3) and (1, 3) */
    states[0][3].q_values[0] = 1.0; //define the Q values for (0, 3)
    states[0][3].q_values[1] = 1.0;
    states[0][3].q_values[2] = 1.0;
    states[0][3].q_values[3] = 1.0;
    states[1][3].q_values[0] = -1.0; //define the Q value for (1, 3)
    states[1][3].q_values[1] = -1.0;
    states[1][3].q_values[2] = -1.0;
    states[1][3].q_values[3] = -1.0;
    
    
    /* Reset the values for the two special states: diamonds (0, 3), pitfall (1, 3). Actually there are no Q-values for these two states as these two states represents the final state of the game. Similarly, for the wall (1, 1), it does not have any state value or Q values. So make sure not to update these three states during your program*/
    states[0][3].state_value = 1.0;
    states[1][3].state_value = -1.0;
    
    
    cur_loc.x = 0;
    cur_loc.y = 2;
    
    
    
    
    
}


RLearning::~RLearning()
{


}

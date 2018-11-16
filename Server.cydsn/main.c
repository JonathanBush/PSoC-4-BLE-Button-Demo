/**
 * One-Way Communication demo server for PSoC 4 BLE.
 *
 * Pressing SW2 on the server Pioneer Kit base board should cause
 * the red LED to light on the client Pioneer Kit base board. 
 * 
 * by Jonathan Bush
 * Arizona State University
 * 2018.11.15
 * 
 * Based on BluetoothCommunication by Harsha Kadekar
 * https://github.com/harsha-kadekar/BluetoothCommunication
 *
 * Released under the Apache License 2.0
 */

#include "project.h"
#include "main.h"
#include "cyapicallbacks.h"

/* this is the button state that we want the client to read */
uint8 button_state;

/*
Name: updateGATTDB
Description: This function will update Server's GATTDB so that client can read the values
             Here I am converting my counter value to RGB value and storing as 3 byte array
             in server's GATT DB. Whenever client do a read on server, the values in GATTDB
             will be sent to client.
Parameters: counter - Number of times button has been pressed or value to be sent to client
Return: - 
*/
void updateGATTDB()
{
    
    CYBLE_GATT_HANDLE_VALUE_PAIR_T		TransmitDataHandle; //This describes the data to be sent to client. 
    
    union ServerBytes Data;              //This is the data to be stored in DB    
    
    //Put that calculated value to the Server DB datastructure.
    Data.values[0] = button_state;

    //Before committing the value to GATTDB, process any previous
    //bluetooth signals.
    CyBle_ProcessEvents();
    
    TransmitDataHandle.attrHandle = 0x000Eu;          //CYBLE_CUSTOMSERVICE_TRANSMITDATA_CHAR_HANDLE in BLE_custom.h
    TransmitDataHandle.value.val = Data.bytes;        //Actual values to be stored
	TransmitDataHandle.value.len = 3;                 //Number of bytes to be transferred or stored
    
    //This function call will store the value to the Server GATTDB. 
    // Any read requests from client after executing below function will 
    // reflect the new values
    CyBle_GattsWriteAttributeValue(&TransmitDataHandle, 
									FALSE, 
									&cyBle_connHandle, 
									CYBLE_GATT_DB_LOCALLY_INITIATED);
    
}

/*
Name: BluetoothEventHandler
Description: This function will handle all the incoming or outgoing BLUETOOH signals/events
Parameters: event - Type of signal like bluetooth started, bluetooth connected to another device,
                disconnected from client, read request, write request, etc.
        eventParams - information related to bluetooth event. Like if client wants to write
                       values, then event will write event and eventParams will have data to be written
return: -

*/
void BluetoothEventHandler(uint32 event, void * eventParam)
{
	
	switch(event)
    {
		case CYBLE_EVT_STACK_ON:
			/* BLE stack is on. Start BLE advertisement */
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
		break;
		
		case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
			/* This event is generated whenever Advertisement starts or stops.
			* The exact state of advertisement is obtained by CyBle_State() */
			if(CyBle_GetState() == CYBLE_STATE_DISCONNECTED)
			{
				CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
			}
            
            if(CyBle_GetState() == CYBLE_STATE_CONNECTED)
            {
                //An indication that devices are connected via bluetooth
                Pin_Red_Write(HIGH);
            }
            
		break;
			
		case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
			/* This event is generated at GAP disconnection. 
			* Restart advertisement */
			CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            Pin_Red_Write(LOW);
			break;

			
	    case CYBLE_EVT_GATT_DISCONNECT_IND:
			/* This event is generated at GATT disconnection */
			//nCounter = 0; //After disconnection resetting my server state.
            updateGATTDB();
            Pin_Red_Write(LOW);
            //After reset, start advertisement - server available for connection with client
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            
			break;	
			
        default:

       	break;
    }   	
}

/*
Name: InitializeSystem
Description: This function will bring your system to ready state or initial state
             Here we switch on components like bluetooth or UART, switch of LEDs
             like that.
Parameters: - 
Return: - 
*/
void InitializeSystem(void)
{
    CySysPmUnfreezeIo();
    
    button_state = 0;
    
	/* Enable Global Interrupt Mask */
	CyGlobalIntEnable;		

    //Start your bluetooth. GeneralEventHandler is the function which will your 
    //bluetooth signals/events.
	CyBle_Start(BluetoothEventHandler);
    
    //Setting the initial values of the Server GATT DB
    updateGATTDB();
 
}

int main(void)
{
    InitializeSystem(); /* initialization routine */
  
    /* loop forever */
    for(;;)
    {        
        CyBle_ProcessEvents(); /* process any pending BLE events first */
    
        button_state = Switch_2_Read(); /* read the current switch state */
        
        updateGATTDB(); /* update the GATT DB so that the client can read the new switch state */
            
        CyDelay(500);   /* wait a bit before checking again */

    }
}


/* [] END OF FILE */

/**
 * One-Way Communication demo client for PSoC 4 BLE.
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

#include <project.h>
#include <main.h>

/* 'list_of_devices' is of type 'CYBLE_GAPC_ADV_REPORT_T' (defined in 
 * BLE_StackGap.h) that is used to store the reports of the peripheral devices
* being scanned by this Central device*/
CYBLE_GAPC_ADV_REPORT_T 	list_of_devices;

/* 'connectPeriphDevice' is a variable of type 'CYBLE_GAP_BD_ADDR_T' (defined in 
* BLE_StackGap.h) and is used to store address of the connected device. */
CYBLE_GAP_BD_ADDR_T 		connectPeriphDevice;

/* 'connHandle' is a variable of type 'CYBLE_CONN_HANDLE_T' (defined in 
* BLE_StackGatt.h) and is used to store the connection handle parameters after
* connecting with the peripheral device. */
CYBLE_CONN_HANDLE_T			connHandle;

/* 'deviceConnected' flag tells the status of connection with BLE peripheral Device */
uint8 deviceConnected = FALSE;

/* 'ble_state' stores the state of connection which is used for updating LEDs */
//uint8 ble_state = BLE_DISCONNECTED;

/* 'address_store' stores the addresses returned by Scanning results */
uint8 address_store[6];

/* 'peripheralAddress' stores the addresses of device presently connected to */
uint8 peripheralAddress[6];

/* 'peripheralFound' flag tells whether the peripheral device with the required Address
 * has been found during scanning or not. */
uint8 peripheralFound = FALSE;

/* 'addedDevices' tells the number of devices that has been added to the list till now. */
uint8 addedDevices = FALSE;

/* 'restartScanning' flag indicates to application whether starting scan API has to be
* called or not */
uint8 restartScanning = FALSE;

//This structure is used to recieve the data from the server side.
union ServerBytes sensor_data;

int led_enable; /* whether the LED should be enabled or not, read from the server */

char buf[200]; //just to hold text values in for writing to UART

/*******************************************************************************
* Function Name: LoadPeripheralDeviceData
********************************************************************************
* Summary:
*        This function stores the address of the peripheral device to which this
* Central device wishes to connect. 
*
* Parameters:
*  void
*
* Return:
*  void
*

*******************************************************************************/
void LoadPeripheralDeviceData(void)
{
	/* This stores the 6-byte BD address of peripheral device to which we have to connect.
       This is Servers MAC address. To know the MAC Address. Program your server, 
       Open your CySmart App in your phone. Serach for devices. It should list "Server BLE"
       device. Below the name it should list the MAC address of that device.
	.*/
    //00A050-BEEB02
		peripheralAddress[5] = 0x00;
		peripheralAddress[4] = 0xA0;
		peripheralAddress[3] = 0x50;
		peripheralAddress[2] = 0xBE;
		peripheralAddress[1] = 0xEB;
		peripheralAddress[0] = 0x02;
             
}

/*******************************************************************************
* Function Name: HandleScanDevices
********************************************************************************
* Summary:
*        This function checks for new devices that have been scanned 
*           sets flag to allow connection with it.
*
* Parameters:
*  scanReport:		parameter of type CYBLE_GAPC_ADV_REPORT_T* returned by BLE
* 					event CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT.
*
* Return:
*  void
*
*******************************************************************************/
void HandleScanDevices(CYBLE_GAPC_ADV_REPORT_T* scanReport)
{
    //Compare scanned device address with the server address
	if(FALSE == memcmp(&peripheralAddress[0], scanReport->peerBdAddr, ADV_ADDR_LEN))
	{
		/* Save the connected device BD Address and Type*/
		connectPeriphDevice.bdAddr[0] = scanReport->peerBdAddr[0];
		connectPeriphDevice.bdAddr[1] = scanReport->peerBdAddr[1];
		connectPeriphDevice.bdAddr[2] = scanReport->peerBdAddr[2];
		connectPeriphDevice.bdAddr[3] = scanReport->peerBdAddr[3];
		connectPeriphDevice.bdAddr[4] = scanReport->peerBdAddr[4];
		connectPeriphDevice.bdAddr[5] = scanReport->peerBdAddr[5];
        
		
		connectPeriphDevice.type = scanReport->peerAddrType;
			
		/* Set the flag to notify application of a connected peripheral device */
		peripheralFound = TRUE;	
		
		/* Stop existing BLE Scan */
		CyBle_GapcStopScan();
	}
	
	
}

/*******************************************************************************
* Function Name: BluetoothEventHandler
********************************************************************************
* Summary:
*        Call back event function to handle various events from BLE stack
*
* Parameters:
*  event:		event returned
*  eventParam:	link to value of the event parameter returned
*
* Return:
*  void
*
*******************************************************************************/
void BluetoothEventHandler(uint32 event, void *eventparam)
{
	/* 'apiResult' is a variable of type 'CYBLE_API_RESULT_T' (defined in 
	* BLE_StackTypes.h) and is used to store the return value from BLE APIs. */
	CYBLE_API_RESULT_T 			apiResult;
	
	/* 'scan_report' is a variable of type 'CYBLE_GAPC_ADV_REPORT_T' (defined in 
	* BLE_StackGap.h) and is used to store report retuned from Scan results. */
	CYBLE_GAPC_ADV_REPORT_T		scan_report;
    
    //This is the structure sent by server for any read requests initiated by client
    CYBLE_GATTC_READ_RSP_PARAM_T *readParams;
	
	switch(event)
	{
		case CYBLE_EVT_STACK_ON:
			
			/* Set start scanning flag to allow calling the API in main loop */
			restartScanning = TRUE;
		break;
			
		case CYBLE_EVT_GAPC_SCAN_START_STOP:
			/* Add relevant code here pertaining to Starting/Stopping of Scan*/
			if(CyBle_GetState() == CYBLE_STATE_DISCONNECTED)
			{	
				if(!peripheralFound)
				{
					restartScanning = TRUE;
				}
			}
		break;
			
		case CYBLE_EVT_GAPC_SCAN_PROGRESS_RESULT:
			/* This event is generated whenever there is a device found*/
			if(CYBLE_STATE_CONNECTED != CyBle_GetState())	
			{
				/* If we are not connected to any peripheral device, then save the new device  
					information so to add it to our list */
				scan_report = *(CYBLE_GAPC_ADV_REPORT_T*) eventparam;
				
				/*Check if the device listed is the one we want to connect, if so flat it for
                 connection and stop scanning*/
				HandleScanDevices(&scan_report);
			}
		break;
			
 		case CYBLE_EVT_GATT_CONNECT_IND:

			/* When the peripheral device is connected, store the connection handle.*/
            connHandle = *(CYBLE_CONN_HANDLE_T *)eventparam;
            break;
			
		case CYBLE_EVT_GATT_DISCONNECT_IND:
			/* When the peripheral device is disconnected, reset variables*/
			
			break;
			
		case CYBLE_EVT_GAP_DEVICE_CONNECTED:
			
			/* The Device is connected now. Start Attributes discovery process.*/
			apiResult = CyBle_GattcStartDiscovery(connHandle);
			
        break;
			
		case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
			/* Reset all saved peripheral Addresses */
				list_of_devices.peerBdAddr[0] = FALSE;
				list_of_devices.peerBdAddr[1] = FALSE;
				list_of_devices.peerBdAddr[2] = FALSE;
				list_of_devices.peerBdAddr[3] = FALSE;
				list_of_devices.peerBdAddr[4] = FALSE;
				list_of_devices.peerBdAddr[5] = FALSE;
			
			/* Reset application Flags on BLE Disconnect */
			addedDevices = FALSE;
			peripheralFound = FALSE;
			deviceConnected = FALSE;
			
			/* Set the flag for rescanning after wakeup */
			restartScanning = TRUE;
                
            //OnOff = 0;
			
			break;
			
	
		case CYBLE_EVT_GATTC_DISCOVERY_COMPLETE:
			/* This event is generated whenever the discovery procedure is complete*/
			
			/*Set the Device connected flag*/
			deviceConnected = TRUE;
			
		break;
            
        case CYBLE_EVT_GATTC_READ_RSP:
			/* This event is generated when the server sends a response to a read request*/
			readParams = (CYBLE_GATTC_READ_RSP_PARAM_T *) eventparam;
            
            led_enable = readParams->value.val[0];
              
		break;
	}
}

/*
Name: InitializeSystem
Description: This function is used to initialize the system. It starts the bluetooth, uart
             puts initial configuration to LEDs etc
Paramters: - 
Return: - 
*/
void InitializeSystem(void)
{
    led_enable = FALSE;
    
	/* Enable global interrupts. */
	CyGlobalIntEnable; 
	
    
	/* Start BLE component with appropriate Event handler function */
	CyBle_Start(BluetoothEventHandler);	
	
	/* Load the BD address of peripheral device to which we have to connect */
	LoadPeripheralDeviceData(); 
    
    sensor_data.values[0] = 0;
    sensor_data.values[1] = 0;
    sensor_data.values[2] = 0;
    
}

/* end borrowed code */

int main(void)
{

    InitializeSystem();
    
    for(;;)
    {
        CyBle_ProcessEvents();
        if(peripheralFound) {
            CyBle_GapcConnectDevice(&connectPeriphDevice);
            CyBle_ProcessEvents();
            peripheralFound = FALSE;
        }
        
        if (restartScanning) {
            restartScanning = FALSE;
            CyBle_GapcStartScan(CYBLE_SCANNING_FAST);
        }
        
        if (deviceConnected) {
            CyBle_GattcReadCharacteristicValue(connHandle, 0x000Eu);    /* read values from server */
        }
       
        Red_LED_Write(led_enable);  /* turn the LED on or off based on the value read from the server */
        
        CyDelay(1000); /* update every second */
        
    }
}

//Project File End

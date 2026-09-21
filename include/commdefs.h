#ifndef _COMMDEFS_H_
#define _COMMDEFS_H_

#define COMM_BUFFER_SIZE	(128)		// Buffer size used for packet communications
#define STACKSHOT_BAUD_RATE	(38400)		// Baud-rate used for the StackShot interface

#define RAIL_STATUS_IDLE		 0x00	// The rail is idle
#define RAIL_STATUS_MOVING		 0x01	// The rail is currently moving
#define RAIL_STATUS_SHUTTER		 0x02	// The shutter is currently firing
#define RAIL_STATUS_USER_ABORTED 0x04	// The user has aborted a move

typedef enum
{
    COMM_STATUS_FAILED,
    COMM_STATUS_SUCCESS,		// The operation was successful
    COMM_STATUS_BUSY,			// The current operation could not begin because it is already in progress
    COMM_STATUS_DATA_MISSING,	// Data was not fully read
	COMM_STATUS_BAD_SYNC,		// The sync-byte was missing in the communiction stream
	COMM_STATUS_BUFFER_OVERRUN,	// The controller returned more data than could be processed
	COMM_STATUS_NOT_EMPTY,		// The requested write operation could not be performed because data was already present (write-once register)
	COMM_STATUS_IO_ERROR,		// An input/output error occured (USB unplugged, bad handle, etc)
	COMM_STATUS_BAD_PARAM,		// One of the parameters passed into the function was rejected by the controller
	COMM_STATUS_NOT_FOUND		// The requested controller could not be found via USB
} comm_status_t;

typedef enum
{
	COMM_RAIL_AXIS_ANY,			// Use any available axis when opening
	COMM_RAIL_AXIS_X,			// Only open the X axis
	COMM_RAIL_AXIS_Y,			// Only open the Y axis
	COMM_RAIL_AXIS_Z,			// Only open the Z axis
	COMM_RAIL_AXIS_UNDEFINED	// The axis stored in the controller is undefined
} comm_rail_axis_t;

typedef enum
{
	CC_RAIL_MOVE = 0x1000,		// Move the rail to the specified position
	CC_RAIL_POSITION_TARGET,	// Desired target position for the rail
	CC_RAIL_POSITION_CURRENT,	// Current position of the rail
	CC_RAIL_POSITION_ZERO,		// Zero out the current position of the rail
	CC_RAIL_SHUTTER_FIRE,		// Fire the shutter control
	CC_RAIL_STATUS,				// Retrieve the current controller status
	CC_RAIL_STOP,				// Stop the rail from moving
	CC_RAIL_MOVE_AT_SPEED,		// Move the rail at the specified speed

	CC_RAIL_CONFIG_NAME = 0x1080,	// The name for the current configuration
	CC_RAIL_CONFIG_BACKLIGHT,		// Backlighting configuration
	CC_RAIL_CONFIG_MODE,			// Operating mode of the controller
	CC_RAIL_CONFIG_UNITS,			// Units -- mm/mils/steps
	CC_RAIL_CONFIG_TORQUE,			// Torque setting for the motor
	CC_RAIL_CONFIG_NUM_STEPS,		// Number of steps to use for a stack
	CC_RAIL_CONFIG_NUM_PULSES,		// Number of pulses on the shutter per step
	CC_RAIL_CONFIG_TOTAL_DISTANCE,	// Total distance config
	CC_RAIL_CONFIG_DISTANCE_PER_STEP,	// Distance to travel per step
	CC_RAIL_CONFIG_SETTLE_TIME,			// Settling time
	CC_RAIL_CONFIG_OFF_TIME,			// Off time between shutter pulses
	CC_RAIL_CONFIG_SPEED,				// Speed that the rail will move
	CC_RAIL_CONFIG_RAMP_TIME,			// Ramp time for the rail
	CC_RAIL_CONFIG_DISTANCE_PER_REV,	// Linear distance per revolution of the motor
	CC_RAIL_CONFIG_SHUTTER_DISABLE,		// Shutter disable feature enabled/disabled
	CC_RAIL_CONFIG_AUTO_RETURN,			// Auto-return feature enabled/disabled
	CC_RAIL_CONFIG_SAVE,				// Save the current configuration
	CC_RAIL_CONFIG_LOAD,				// Load the specified configuration
	CC_RAIL_CONFIG_AXIS,				// Get/set the controllers configured axis
	CC_RAIL_CONFIG_TIMELAPSE,			// Time-lapse feature enabled/disabled
	CC_RAIL_CONFIG_PULSE_TIME,			// On time of the shutter pulse
	CC_RAIL_CONFIG_BACKLASH,			// Rail backlash configuration
	CC_RAIL_CONFIG_HOLDING_TORQUE,		// Holding torque for when the rail isn't moving
	CC_RAIL_CONFIG_SPEED_MOVE,			// Speed used by FWD/BACK buttons
	CC_RAIL_CONFIG_POLARITY,			// Direction polarity
	CC_RAIL_CONFIG_IO_MODE,				// IO mode -- Normal, master, slave
	CC_RAIL_CONFIG_IO_DIR,				// IO mode = Master, FWD or BACK
	CC_RAIL_CONFIG_ROTARY_BACKLASH,		// Backlash for rotary tables
	CC_RAIL_CONFIG_ROTARY_RATIO,		// Rotary table gear ratio
	CC_RAIL_CONFIG_ROTARY_DEGREES,		// Degrees to move per step

	CC_RESET = 0x1100,				// Reset the controller
	CC_REFLASH,						// Start reflash
	CC_SOFTWARE_STRING,				// Software string (human readable)
	CC_SOFTWARE_ID,					// Sftware identifier
	CC_HARDWARE_ID,					// Hardware identifer
	CC_BOOTLOADER_ID,				// Bootloader identifer
	CC_SOFTWARE_CHECKSUM,			// Software checksum
	CC_SERIAL_NUMBER,				// The serial number of the device
	CC_NVM_ACCESS,					// NVM register access
	CC_NAND_ACCESS,
	CC_NOR_ACCESS,
	CC_PING,
	CC_LOG,
	CC_WIFI,
	CC_CLOSE

} comm_cmd_t;

typedef enum
{
	COMM_ACTION_MIN,
	COMM_ACTION_READ = COMM_ACTION_MIN,	// Read the specified command
	COMM_ACTION_WRITE,					// Write the specified command
	COMM_ACTION_RSP_OK,					// Controller responded with OK
	COMM_ACTION_BAD_PARAM,				// A bad parameter was passed to the controller
	COMM_ACTION_UNSUPPORTED_ACTION,		// The action specified is invalid
	COMM_ACTION_UNSUPPORTED_CMD,		// The command passed in is invalid
	COMM_ACTION_FAILED,					// The command failed (no further information available)
	COMM_ACTION_NOT_EMPTY,				// The write operation was to a write-once register and it is no longer empty
	COMM_ACTION_BUSY,					// The controller is already performing the specified action
	COMM_ACTION_MAX
} comm_action_t;

typedef enum
{
	COMM_RAIL_DIR_MIN,
	COMM_RAIL_DIR_FWD = COMM_RAIL_DIR_MIN,	// Move the rail in the forward direction
	COMM_RAIL_DIR_BACK,						// Move the rail in the backward direction
	COMM_RAIL_DIR_MAX
} comm_rail_dir_t;

typedef enum
{
	COMM_RAIL_UNITS_MIN,
	COMM_RAIL_UNITS_ENGLISH = COMM_RAIL_UNITS_MIN,	// English/mils
	COMM_RAIL_UNITS_METRIC,							// Metric/mm
	COMM_RAIL_UNITS_STEPS,							// Motor steps
	COMM_RAIL_UNITS_DEGREES,						// Degrees for rotary moves
	COMM_RAIL_UNITS_MAX
} comm_rail_units_t;

typedef enum
{
	COMM_RAIL_MODE_MIN,
	COMM_RAIL_MODE_AUTO_STEP = COMM_RAIL_MODE_MIN,	// Automatic step mode
	COMM_RAIL_MODE_AUTO_DIST,						// Automatic distance mode
	COMM_RAIL_MODE_TOTAL_DISTANCE,					// Total distance mode
	COMM_RAIL_MODE_DISTANCE_PER_STEP,				// distance per step mode
	COMM_RAIL_MODE_MANUAL,							// Manual mode
	COMM_RAIL_MODE_CONTINUOUS,						// Continuous mode
	SETTING_MODE_MANUAL_DISTANCE,					// Manual distance mode
	SETTING_MODE_ROTARY,							// Rotary mode
	COMM_RAIL_MODE_MAX
} comm_rail_mode_t;

typedef enum
{
	SETTING_IO_MIN,
	SETTING_IO_STANDALONE = SETTING_IO_MIN,
	SETTING_IO_MASTER,
	SETTING_IO_SLAVE,
	SETTING_IO_MAX
} comm_rail_io_mode_t;

typedef enum
{
	SETTING_IO_DIR_MIN,
	SETTING_IO_DIR_FWD = SETTING_IO_DIR_MIN,
	SETTING_IO_DIR_BACK,
	SETTING_IO_DIR_MAX
} comm_rail_io_dir_t;

typedef enum
{
	SETTING_MOVE_POLARITY_NORMAL,
	SETTING_MOVE_POLARITY_REVERSE
} comm_rail_move_polarity_t;


#endif /* _COMMDEFS_H_ */
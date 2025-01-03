#ifndef MODBUS_COMM_H
#define MODBUS_COMM_H

#include <stdbool.h>
#include <modbus.h>
#include "specification_genel.h"
#include "sgl_types.h"

// Constants
#define MAX_LINE 2048
#define MAX_MAPPINGS 1024
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 502
#define DEFAULT_SLAVE_ID 1
#define CONFIG_FILE "config.ini"
#define LOG_FILE "trackcircuit.log"
#define EXPORT_FILE "mappings.csv"
#define CONNECTION_RETRY_INTERVAL 5
#define MODBUS_READ_INTERVAL 2
#define MODBUS_TIMEOUT 100000 // 100 ms Connection timeout
#define CONTEXT_STRUCT_NAME specification_typ_genel //Context struct name from [ansys_project_name]_[layer_name].h

// Structures
typedef struct {
    char name[50];
    int address;
    bool value;
    bool prev_value;
    SGLbool* struct_ptr;
} ModbusMapping;

typedef struct {
    char server_ip[20];
    int port;
    int slave_id;
    ModbusMapping input_mappings[MAX_MAPPINGS];
    ModbusMapping output_mappings[MAX_MAPPINGS];
    int input_count;
    int output_count;
    int max_input_address;
    int max_output_address;
} ModbusConfig;

// Function prototypes
void write_log(const char* format, ...);
ModbusConfig* load_config(const char* filename);
void init_mappings(CONTEXT_STRUCT_NAME *context);
bool init_modbus_communication(CONTEXT_STRUCT_NAME *context);
bool read_modbus_values(CONTEXT_STRUCT_NAME *context);
bool update_modbus_values_all(CONTEXT_STRUCT_NAME *context);
bool update_modbus_values(CONTEXT_STRUCT_NAME *context);
bool export_mappings(const char* filename);
void cleanup_modbus(void);

#endif // MODBUS_COMM_H
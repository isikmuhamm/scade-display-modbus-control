#include "modbus_comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

// Global variables
static modbus_t *ctx = NULL;
static ModbusConfig *config = NULL;
static FILE *log_file = NULL;
static time_t last_read_time = 0;

// Logging function
void write_log(const char* format, ...) {
    if (!log_file) {
        log_file = fopen(LOG_FILE, "a");
        if (!log_file) return;
    }
    
    time_t now;
    time(&now);
    char timestamp[26];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(log_file, "[%s] ", timestamp);
    
    va_list args;
    va_start(args, format);
    vfprintf(log_file, format, args);
    va_end(args);
    
    fprintf(log_file, "\n");
    fflush(log_file);
}

// Config file loaded here
ModbusConfig* load_config(const char* filename) {
    FILE* file = fopen(filename, "r");
    ModbusConfig* cfg = malloc(sizeof(ModbusConfig));
    if (!cfg) {
        write_log("ERROR: Memory allocation failed for config");
        if (file) fclose(file);
        return NULL;
    }

    // Initialize all values including max addresses
    memset(cfg, 0, sizeof(ModbusConfig));
    strcpy(cfg->server_ip, DEFAULT_IP);
    cfg->port = DEFAULT_PORT;
    cfg->slave_id = DEFAULT_SLAVE_ID;
    cfg->max_input_address = 0;
    cfg->max_output_address = 0;

    // If no config file, return with default values
    if (!file) {
        FILE* new_file = fopen(filename, "w");
        if (new_file) {
            fprintf(new_file, "[ModbusConfig]\nserver_ip=%s\nport=%d\nslave_id=%d\n\n", 
                    DEFAULT_IP, DEFAULT_PORT, DEFAULT_SLAVE_ID);
            fclose(new_file);
            write_log("Default config file created");
        } else {
            write_log("ERROR: Unable to create default config file");
        }
        return cfg;
    }

    char line[MAX_LINE];
    int section = 0; // 0: ModbusConfig, 1: InputMappings, 2: OutputMappings

    while (fgets(line, MAX_LINE, file)) {
        line[strcspn(line, "\r\n")] = 0;
        
        if (line[0] == '[') {
            if (strstr(line, "[ModbusConfig]")) section = 0;
            else if (strstr(line, "[InputMappings]")) section = 1;
            else if (strstr(line, "[OutputMappings]")) section = 2;
            continue;
        }

        if (line[0] == '\0' || line[0] == ';') continue;

        char key[50], value[50];
        if (sscanf(line, "%[^=]=%[^\n]", key, value) == 2) {
            char* k = key;
            while (*k == ' ') k++;
            char* v = value;
            while (*v == ' ') v++;

            switch(section) {
                case 0: // ModbusConfig
                    if (strcmp(k, "server_ip") == 0) strcpy(cfg->server_ip, v);
                    else if (strcmp(k, "port") == 0) cfg->port = atoi(v);
                    else if (strcmp(k, "slave_id") == 0) cfg->slave_id = atoi(v);
                    break;
 
                case 1: // InputMappings
                    if (cfg->input_count < MAX_MAPPINGS) {
                        int address = atoi(v);
                        strcpy(cfg->input_mappings[cfg->input_count].name, k);
                        cfg->input_mappings[cfg->input_count].address = address;
                        // Update max input address
                        if (address > cfg->max_input_address) {
                            cfg->max_input_address = address;
                        }
                        write_log("Added input mapping: %s = %d", k, address);
                        cfg->input_count++;
                    }
                    break;

                case 2: // OutputMappings
                    if (cfg->output_count < MAX_MAPPINGS) {
                        int address = atoi(v);
                        strcpy(cfg->output_mappings[cfg->output_count].name, k);
                        cfg->output_mappings[cfg->output_count].address = address;
                        // Update max output address
                        if (address > cfg->max_output_address) {
                            cfg->max_output_address = address;
                        }
                        write_log("Added output mapping: %s = %d", k, address);
                        cfg->output_count++;
                    }
                    break;
            }
        }
    }

    fclose(file);
    write_log("Config loaded successfully.");
    return cfg;
}

// Modbus names and adresses are matched with program variables here.
void init_mappings(CONTEXT_STRUCT_NAME *context) {
    for (int i = 0; i < config->input_count; i++) {
        // Max adress value is found
        if(config->input_mappings[i].address > config->max_input_address) config->max_input_address = config->input_mappings[i].address;
        // Modbus Input Mappings Adress and Name Pointer Assignments
        if (strcmp(config->input_mappings[i].name, "out_RT01_Accept") == 0) config->input_mappings[i].struct_ptr = &context->out_RT01_Accept;
        if (strcmp(config->input_mappings[i].name, "out_RT01_Reject") == 0) config->input_mappings[i].struct_ptr = &context->out_RT01_Reject;
        if (strcmp(config->input_mappings[i].name, "out_RT01_RejectAck") == 0) config->input_mappings[i].struct_ptr = &context->out_RT01_RejectAck;
        if (strcmp(config->input_mappings[i].name, "out_RT01_Request") == 0) config->input_mappings[i].struct_ptr = &context->out_RT01_Request;
        if (strcmp(config->input_mappings[i].name, "out_RT01_Reserve") == 0) config->input_mappings[i].struct_ptr = &context->out_RT01_Reserve;
    }
    
    for (int i = 0; i < config->output_count; i++) {
        // Max adress value is found
        if(config->output_mappings[i].address > config->max_output_address) config->max_output_address = config->output_mappings[i].address;
        //Modbus Output Mappings Adress and Name Pointer Assignments
        if (strcmp(config->output_mappings[i].name, "in_RT01_RejectAck") == 0) config->output_mappings[i].struct_ptr = &context->in_RT01_RejectAck;
        if (strcmp(config->output_mappings[i].name, "in_RT01_Request") == 0) config->output_mappings[i].struct_ptr = &context->in_RT01_Request;
        if (strcmp(config->output_mappings[i].name, "in_TC03_I_Occupied_hws") == 0) config->output_mappings[i].struct_ptr = &context->in_TC03_I_Occupied_hws;
        if (strcmp(config->output_mappings[i].name, "in_RT02_RejectAck") == 0) config->output_mappings[i].struct_ptr = &context->in_RT02_RejectAck;
        if (strcmp(config->output_mappings[i].name, "in_RT02_Request") == 0) config->output_mappings[i].struct_ptr = &context->in_RT02_Request;
    }
    write_log("Modbus Input and Output Mappings initialized.");
}

// Modbus connection is established here
bool init_modbus_communication(CONTEXT_STRUCT_NAME *context) {
    if (config == NULL) {
        config = load_config(CONFIG_FILE);
        if (config == NULL) {
            write_log("ERROR: Failed to load config");
            return false;
        }
    }

    if (ctx != NULL) return true;  // Already connected

    ctx = modbus_new_tcp(config->server_ip, config->port);
    if (ctx == NULL) {
        write_log("ERROR: Failed to create modbus context");
        return false;
    }

    modbus_set_response_timeout(ctx, 0, 500000);  // 500ms timeout
    modbus_set_slave(ctx, config->slave_id);

    if (modbus_connect(ctx) == -1) {
        write_log("ERROR: Connection failed: %s", modbus_strerror(errno));
        modbus_free(ctx);
        ctx = NULL;
        return false;
    }

    // Add context parameter when calling init_mappings
    init_mappings(context);

    write_log("Connected to Modbus server at %s:%d", config->server_ip, config->port);
    return true;
}

// Modbus values readed and assigned into struct
bool read_modbus_values(CONTEXT_STRUCT_NAME *context) {
    time_t current_time = time(NULL);
    
    // Check if 1 second has passed
    if (current_time - last_read_time < 1) {
        return true; // Use existing values
    }
    
    if (!init_modbus_communication(context)) return false;
    if (!context || !config) {
        write_log("ERROR: Null context or config in read_modbus_values");
        return false;
    }

    // Allocate buffers for input and output bits
    uint8_t* input_bits = calloc(config->max_input_address + 1, sizeof(uint8_t));
    uint8_t* output_bits = calloc(config->max_output_address + 1, sizeof(uint8_t));
    
    if (!input_bits || !output_bits) {
        write_log("ERROR: Memory allocation failed");
        free(input_bits);
        free(output_bits);
        return false;
    }

    // Read all input bits in one request
    if (modbus_read_input_bits(ctx, 0, config->max_input_address + 1, input_bits) == -1) {
        write_log("ERROR: Failed to read input bits: %s", modbus_strerror(errno));
        free(input_bits);
        free(output_bits);
        return false;
    }

    // Read all output bits in one request
    if (modbus_read_bits(ctx, 0, config->max_output_address + 1, output_bits) == -1) {
        write_log("ERROR: Failed to read output bits: %s", modbus_strerror(errno));
        free(input_bits);
        free(output_bits);
        return false;
    }

    // Update input mappings
    for (int i = 0; i < config->input_count; i++) {
        int addr = config->input_mappings[i].address;
        bool value = input_bits[addr];
        
        config->input_mappings[i].prev_value = config->input_mappings[i].value;
        config->input_mappings[i].value = value;
        
        if (config->input_mappings[i].struct_ptr) {
            *(config->input_mappings[i].struct_ptr) = value;
            write_log("Read input %s = %d from address %d", 
                     config->input_mappings[i].name, value, addr);
        }
    }

    // Update output mappings
    for (int i = 0; i < config->output_count; i++) {
        int addr = config->output_mappings[i].address;
        bool value = output_bits[addr];
        
        config->output_mappings[i].prev_value = config->output_mappings[i].value;
        config->output_mappings[i].value = value;
        
        if (config->output_mappings[i].struct_ptr) {
            *(config->output_mappings[i].struct_ptr) = value;
            write_log("Read output %s = %d from address %d", 
                     config->output_mappings[i].name, value, addr);
        }
    }

    free(input_bits);
    free(output_bits);
    last_read_time = current_time;
    return true;
}

// Struct values readed and assigned into modbus all at once at change
bool update_modbus_values_all(CONTEXT_STRUCT_NAME *context) {
    if (!read_modbus_values(context)) return false;
    if (!context || !config) {
        write_log("ERROR: Null context or config");
        return false;
    }

    // Allocate buffer for output bits
    uint8_t* output_bits = calloc(config->max_output_address + 1, sizeof(uint8_t));
    if (!output_bits) {
        write_log("ERROR: Memory allocation failed");
        return false;
    }

    bool values_changed = false;

    // Fill buffer with current values
    for (int i = 0; i < config->output_count; i++) {
        if (!config->output_mappings[i].struct_ptr) continue;
        
        int addr = config->output_mappings[i].address;
        bool current_value = *(config->output_mappings[i].struct_ptr);
        output_bits[addr] = current_value ? 1 : 0;

        // Check if value changed
        if (config->output_mappings[i].prev_value != current_value) {
            values_changed = true;
        }
    }

    // Write to Modbus if any value changed
    if (values_changed) {
        if (modbus_write_bits(ctx, 0, config->max_output_address + 1, output_bits) == -1) {
            write_log("ERROR: Failed to write outputs: %s", modbus_strerror(errno));
            free(output_bits);
            return false;
        }

        // Update previous values after successful write
        for (int i = 0; i < config->output_count; i++) {
            if (!config->output_mappings[i].struct_ptr) continue;
            
            bool current_value = *(config->output_mappings[i].struct_ptr);
            if (config->output_mappings[i].prev_value != current_value) {
                write_log("Updated %s: %d -> %d at address %d", 
                         config->output_mappings[i].name,
                         config->output_mappings[i].prev_value,
                         current_value,
                         config->output_mappings[i].address);
                         
                config->output_mappings[i].prev_value = current_value;
                config->output_mappings[i].value = current_value;
            }
        }
    }

    free(output_bits);
    return true;
}

// Struct values readed and assigned into modbus one by one at change
bool update_modbus_values(CONTEXT_STRUCT_NAME *context) {
    if (!read_modbus_values(context)) return false;
    if (!context || !config) {
        write_log("ERROR: Null context or config");
        return false;
    }

    bool any_write_success = false;

    // Check each output mapping for changes
    for (int i = 0; i < config->output_count; i++) {
        if (!config->output_mappings[i].struct_ptr) continue;
        
        bool current_value = *(config->output_mappings[i].struct_ptr);
        int addr = config->output_mappings[i].address;

        // Write only if value changed
        if (config->output_mappings[i].prev_value != current_value) {
            if (modbus_write_bit(ctx, addr, current_value ? 1 : 0) == -1) {
                write_log("ERROR: Failed to write bit at address %d: %s", 
                         addr, modbus_strerror(errno));
                continue;
            }

            write_log("Updated %s: %d -> %d at address %d", 
                     config->output_mappings[i].name,
                     config->output_mappings[i].prev_value,
                     current_value,
                     addr);
            
            // Update previous value after successful write
            config->output_mappings[i].prev_value = current_value;
            config->output_mappings[i].value = current_value;
            any_write_success = true;
        }
    }

    return any_write_success;
}

// Export mappings to a CSV file (Not using yet)
bool export_mappings(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) return false;

    fprintf(file, "Type,Name,Address,Current Value\n");

    for (int i = 0; i < config->input_count; i++) {
        fprintf(file, "Input,%s,%d,%d\n", 
                config->input_mappings[i].name,
                config->input_mappings[i].address,
                config->input_mappings[i].value);
    }

    for (int i = 0; i < config->output_count; i++) {
        fprintf(file, "Output,%s,%d,%d\n", 
                config->output_mappings[i].name,
                config->output_mappings[i].address,
                config->output_mappings[i].value);
    }

    fclose(file);
    return true;
}

// Cleanup Modbus connection and free memory in a cause of error (Not using yet)
void cleanup_modbus(void) {
    if (ctx) {
        modbus_flush(ctx);
        modbus_close(ctx);
        modbus_free(ctx);
        ctx = NULL;
        write_log("Modbus connection closed");
    }
    if (config) {
        free(config);
        config = NULL;
        write_log("Config freed");
    }
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}
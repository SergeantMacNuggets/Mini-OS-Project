char *input_registers[3]={NULL};
            string_register = strtok(NULL, " ");
            for(int i=0; i<3;i++) {
                input_registers[i] = string_register;
                string_register = strtok(NULL," ");
            }
            printf("%s %s %s\n", input_registers[0], input_registers[1], input_registers[2]);
            break;
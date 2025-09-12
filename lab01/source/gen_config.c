#include "config.h"

int main(int argc, char *argv[])
{
    // Read file name from arguments
	const char *file_name = get_file_name(argc, argv, "test_config");

    // Experiment configuration
    Config cfg;

    // Read configurations from user
    read_config_from_std(&cfg);

    // Write configuration to file
    if (write_config_to_file(file_name, &cfg)) {
        printf("Configuration saved to '%s'\n", file_name);
    } else {
        printf("Failed to save configuration.\n");
    }

    return 0;
}
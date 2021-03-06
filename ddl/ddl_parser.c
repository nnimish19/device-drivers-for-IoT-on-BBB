﻿#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#if defined(LIBXML_XPATH_ENABLED) && defined(LIBXML_SAX1_ENABLED)

// Global variables read from the XML file
int check_analog=0;
char signal_id[10];
char operation[7];
char type[10];
char measurement[10];
char computation_type[10];
char computation_expr[80];
char pin_no[5];

// function declarations
static void usage(const char *name);
char *trimwhitespace(char *str);
int parse_and_generate(const char *name);

int main(int argc, char **argv) {
	/* Parse command line and process file */
	if (argc < 2) {
		fprintf(stderr, "Error: wrong number of arguments.\n");
		usage(argv[0]);
		return (-1);
	}

	/* Init libxml */
	xmlInitParser();
	// LIBXML_TEST_VERSION

	int i;
	for (i = 1; i < argc; i++)
	{
		parse_and_generate(argv[i]);
	}

	/* Shutdown libxml */
	xmlCleanupParser();

	return 0;
}

/**
 * usage:
 * @name:		the program name.
 *
 * Prints usage information.
 */
static void usage(const char *name) {
	assert(name);

	fprintf(stderr, "Usage: %s device1 device2 ...\n",
			name);
	fprintf(stderr, "where device_i is used by treating ./device_i.xml as a DDL xml file\n");
}

int get_content(xmlXPathContextPtr xpathCtx, const char *xpathstr, char *target)
{
	xmlXPathObjectPtr xpathObj;
	const xmlChar *xpathexp = (const xmlChar *)xpathstr;

	xpathObj = xmlXPathEvalExpression(xpathexp, xpathCtx);
	if (xpathObj == NULL || xpathObj->nodesetval==NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expressions: %s\n", xpathexp);
		return -1;
	}
	if(xpathObj->nodesetval->nodeTab == NULL){
		strcpy(target, "");
		xmlXPathFreeObject(xpathObj);
		return 0;
	}
	xmlNodePtr node = xpathObj->nodesetval->nodeTab[0];
	strcpy(target, trimwhitespace((char*)node->content));
	xmlXPathFreeObject(xpathObj);
	return 0;
}

int get_attribute_value(xmlXPathContextPtr xpathCtx, const char *xpathstr, char *target)
{
	xmlXPathObjectPtr xpathObj;
	const xmlChar *xpathexp = (const xmlChar *)xpathstr;

	xpathObj = xmlXPathEvalExpression(xpathexp, xpathCtx);
	if (xpathObj == NULL) {
		fprintf(stderr, "Error: unable to evaluate xpath expressions: %s\n", xpathexp);
		return -1;
	}
	xmlNodePtr node = xpathObj->nodesetval->nodeTab[0];
	strcpy(target, (char*)node->children->content);
	xmlXPathFreeObject(xpathObj);
	return 0;
}


int parse(const char *name)
{
	char filename[30];
	sprintf(filename, "./%s.xml", name);

	xmlDocPtr doc;
	xmlXPathContextPtr xpathCtx;

	assert(filename);

	/* Load XML document */
	doc = xmlParseFile(filename);
	if (doc == NULL) {
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
		return (-1);
	}

	/* Create xpath evaluation context */
	xpathCtx = xmlXPathNewContext(doc);
	if (xpathCtx == NULL) {
		fprintf(stderr, "Error: unable to create new XPath context\n");
		xmlFreeDoc(doc);
		return (-1);
	}

	/* Evaluate xpath expression */
	if (get_attribute_value(xpathCtx, "/DDL/Sensor/Interface/Signal/@id", signal_id) == -1 ||
		get_content(xpathCtx, "/DDL/Sensor/Interface/Signal/Operation/text()", operation) == -1 ||
		get_content(xpathCtx, "/DDL/Sensor/Interface/Signal/Type/text()", type) == -1 ||
		get_content(xpathCtx, "/DDL/Sensor/Interface/Signal/Measurement/text()", measurement) == -1 ||
		get_content(xpathCtx, "/DDL/Sensor/Interface/Signal/Pin/text()", pin_no) == -1 ||
		get_content(xpathCtx, "/DDL/Sensor/Interface/Reading/Computation/Type/text()", computation_type) == -1 ||
		get_content(xpathCtx, "/DDL/Sensor/Interface/Reading/Computation/Expression/text()", computation_expr) == -1) {
		xmlXPathFreeContext(xpathCtx);
		xmlFreeDoc(doc);
		return (-1);
	}
	if (!strcmp(type, "Analog")){
		check_analog=1;
	}
	else{
		check_analog=0;
	}

	/* Cleanup */
	xmlXPathFreeContext(xpathCtx);
	xmlFreeDoc(doc);

	return 0;
}

int generate(const char *folder, const char *name)
{
	char filename[30];
	FILE *fp;

	if (!strcmp(operation, "Input")) {
		sprintf(filename, "%s%sread.c", folder, name);
	}
	else if (!strcmp(operation, "Output")) {
		sprintf(filename, "%s%swrite.c", folder, name);
	}
	else {
		return -1;
	}

    fp = fopen(filename, "w+");
	fprintf(fp, "/* This file is automatically generated according to %s.xml */\n", name);
	fprintf(fp, "#include <xinu.h>\n\n");
	fprintf(fp, "int adc_channel = %s; // select the pin to which the sensor is connected\n\n", pin_no);
	if (!strcmp(operation, "Input")) {
		fprintf(fp, "devcall %sread(struct dentry *devptr, char *buffer, int32 count) {\n", name);
	}
	else if (!strcmp(operation, "Output")) {
		fprintf(fp, "devcall %swrite(struct dentry *devptr, char *buffer, int32 count) {\n", name);
	}

	fprintf(fp, "\tfloat %s;\n", signal_id);
	fprintf(fp, "\tchar data[4];\n");
	fprintf(fp, "\tdevcall retval;\n");
	fprintf(fp, "\tstruct dentry *devptr1;\n\n");

	if (!strcmp(type, "Analog")) {
		fprintf(fp, "\tdevptr1 = (struct dentry *)&devtab[ADC];\n");
	}
	else if (!strcmp(type, "Digital")) {
		fprintf(fp, "\tdevptr1 = (struct dentry *)&devtab[GPIO];\n");
	}

	fprintf(fp, "\tretval = (*devptr1->dvread)(devptr1, data, 4);\n");
	fprintf(fp, "\tif (retval == SYSERR)\n");
	fprintf(fp, "\t\treturn SYSERR;\n\n");
	fprintf(fp, "\t%s = *(unsigned int*)data;\n", signal_id);

	if (!strcmp(computation_type, "Formula")) {
		fprintf(fp, "\t%s;\n\n", computation_expr);
	}

	fprintf(fp, "\t*(float*)buffer = %s;\n", signal_id);
	fprintf(fp, "\treturn retval;\n");
	fprintf(fp, "}\n");

    fclose(fp);

    return 0;
}

int generate_digital(const char *folder, const char *name){
	char filename[30];
	FILE *fp;

	if (!strcmp(operation, "Output")) {
		sprintf(filename, "%s%swrite.c", folder, name);

        fp = fopen(filename, "w+");
        fprintf(fp, "/* This file is automatically generated according to %s.xml */\n", name);
        fprintf(fp, "#include <xinu.h>\n\n");
        fprintf(fp, "devcall %swrite(struct dentry *devptr, char *buffer, int32 count) {\n\n", name);
        fprintf(fp, "\tif(count!=1) {\n");
        fprintf(fp, "\t\treturn 0;\n");
        fprintf(fp, "\t}\n\n");
        fprintf(fp, "\tunsigned int base_address =  *((unsigned int*)(&devptr->dvcsr));\n");
        fprintf(fp, "\tint gpio_instance_pin_number = %s;\n\n",pin_no);
        fprintf(fp, "\tGPIO1PinMuxSetup(gpio_instance_pin_number);\n");
        fprintf(fp, "\tGPIODirModeSet(base_address, gpio_instance_pin_number, GPIO_DIR_OUTPUT);\n\n");
        fprintf(fp, "\tif(buffer[0]=='1') {\n");
        fprintf(fp, "\t\tGPIOPinWrite(base_address, gpio_instance_pin_number, GPIO_PIN_HIGH);\n");
        fprintf(fp, "\t\treturn 1;\n");
        fprintf(fp, "\t}\n");
        fprintf(fp, "\telse if(buffer[0]=='0') {\n");
        fprintf(fp, "\t\tGPIOPinWrite(base_address, gpio_instance_pin_number, GPIO_PIN_LOW);\n");
        fprintf(fp, "\t\treturn 1;\n");
        fprintf(fp, "\t}\n\n");
        fprintf(fp, "\treturn 0;\n");
        fprintf(fp, "}\n");
        fclose(fp);
	}
        else
        {
		sprintf(filename, "%s%sread.c", folder, name);

		fp = fopen(filename, "w+");
		fprintf(fp, "/* This file is automatically generated according to %s.xml */\n", name);
		fprintf(fp, "#include <xinu.h>\n\n");
		fprintf(fp, "devcall %sread(struct dentry *devptr, char *buffer, int32 count) {\n\n", name);
	        //fprintf(fp, "\tdevptr1 = (struct dentry *)&devtab[GPIO];\n");
		fprintf(fp, "\tif(count!=1) {\n");
		fprintf(fp, "\t\treturn 0;\n");
		fprintf(fp, "\t}\n\n");
		fprintf(fp, "\tunsigned int baseaddress =  *((unsigned int*)(&devptr->dvcsr));\n\n");
		fprintf(fp, "\tGPIO1ModuleClkConfig();\n");
		fprintf(fp, "\tGPIOModuleEnable(baseaddress);\n");
		fprintf(fp, "\tGPIOModuleReset(baseaddress);\n");
		fprintf(fp, "\tGPIO1PinMuxSetup(%s);\n",pin_no);
		fprintf(fp, "\tGPIODirModeSet(baseaddress, %s, GPIO_DIR_INPUT);\n\n", pin_no);
		fprintf(fp, "\tint r = GPIOPinRead(baseaddress, %s);\n", pin_no);
		fprintf(fp, "\tif(r==0)\n");
		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\tbuffer[0]='0';\n");
		fprintf(fp, "\t}\n");
		fprintf(fp, "\telse\n");
		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\tbuffer[0]='1';\n");
		fprintf(fp, "\t}\n\n");
		fprintf(fp, "\treturn 1;\n");
		fprintf(fp, "}\n");
		fclose(fp);
        }


	return 0;
}


int parse_and_generate(const char *name)
{
	if (parse(name) == -1) {
		fprintf(stderr, "Error: unable to parse the xml file.\n");
		return -1;
	}

	if(check_analog){
		fprintf(stderr, "inside device\n");
		if (generate("../device/ext/", name) == -1) {
			fprintf(stderr, "Error: unable to generate the device driver for %s.\n", name);
			return -1;
		}
	}
	else{
		fprintf(stderr, "inside device\n");
		if (generate_digital("../device/ext/", name) == -1) {
			fprintf(stderr, "Error: unable to generate the device driver for %s.\n", name);
			return -1;
		}
	}

	return (0);
}

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trimwhitespace(char *str)
{
	char *end;

	// Trim leading space
	while(isspace((unsigned char)*str)) str++;

	if(*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace((unsigned char)*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}

#else
int main(void) {
    fprintf(stderr, "XPath support not compiled in\n");
    exit(1);
}
#endif

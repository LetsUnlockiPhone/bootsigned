/*
 * img3.c
 * Written and Improved by Ritvik Choudhary <ritvik@outlook.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "img3.h"
#include "common.h"
#include "idevicerestore.h"

img3_file* img3_parse_file(char* data, int size) {
	int data_offset = 0;
	img3_element* element;
	img3_header* header = (img3_header*) data;
	if (header->signature != kImg3Container) {
		error("ERROR: Invalid IMG3 file\n");
		return NULL;
	}

	img3_file* image = (img3_file*) malloc(sizeof(img3_file));
	if (image == NULL) {
		error("ERROR: Unable to allocate memory for IMG3 file\n");
		return NULL;
	}
	memset(image, '\0', sizeof(img3_file));
	image->idx_ecid_element = -1;
	image->idx_shsh_element = -1;
	image->idx_cert_element = -1;

	image->header = (img3_header*) malloc(sizeof(img3_header));
	if (image->header == NULL) {
		error("ERROR: Unable to allocate memory for IMG3 header\n");
		img3_free(image);
		return NULL;
	}
	memcpy(image->header, data, sizeof(img3_header));
	data_offset += sizeof(img3_header);

	img3_element_header* current = NULL;
	while (data_offset < size) {
		current = (img3_element_header*) &data[data_offset];
		switch (current->signature) {
		case kTypeElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse TYPE element\n");
				img3_free(image);
				return NULL;
			}
			image->elements[image->num_elements++] = element;
			debug("Parsed TYPE element\n");
			break;

		case kDataElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse DATA element\n");
				img3_free(image);
				return NULL;
			}
			image->elements[image->num_elements++] = element;
			debug("Parsed DATA element\n");
			break;

		case kVersElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse VERS element\n");
				img3_free(image);
				return NULL;
			}
			image->elements[image->num_elements++] = element;
			debug("Parsed VERS element\n");
			break;

		case kSepoElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse SEPO element\n");
				img3_free(image);
				return NULL;
			}
			image->elements[image->num_elements++] = element;
			debug("Parsed SEPO element\n");
			break;

		case kBordElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse BORD element\n");
				img3_free(image);
				return NULL;
			}
			image->elements[image->num_elements++] = element;
			debug("Parsed BORD element\n");
			break;

		case kChipElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse CHIP element\n");
				img3_free(image);
				return NULL;
			}
			image->elements[image->num_elements++] = element;
			debug("Parsed CHIP element\n");
			break;

		case kKbagElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse first KBAG element\n");
				img3_free(image);
				return NULL;
			}
			image->elements[image->num_elements++] = element;
			debug("Parsed KBAG element\n");
			break;

		case kEcidElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse ECID element\n");
				img3_free(image);
				return NULL;
			}
			image->idx_ecid_element = image->num_elements;
			image->elements[image->num_elements++] = element;
			debug("Parsed ECID element\n");
			break;

		case kShshElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse SHSH element\n");
				img3_free(image);
				return NULL;
			}
			image->idx_shsh_element = image->num_elements;
			image->elements[image->num_elements++] = element;
			debug("Parsed SHSH element\n");
			break;

		case kCertElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse CERT element\n");
				img3_free(image);
				return NULL;
			}
			image->idx_cert_element = image->num_elements;
			image->elements[image->num_elements++] = element;
			debug("Parsed CERT element\n");
			break;

		case kUnknElement:
			element = img3_parse_element(&data[data_offset]);
			if (element == NULL) {
				error("ERROR: Unable to parse UNKN element\n");
				img3_free(image);
				return NULL;
			}
			image->elements[image->num_elements++] = element;
			debug("Parsed UNKN element\n");
			break;

		default:
			error("ERROR: Unknown IMG3 element type %08x\n", current->signature);
			img3_free(image);
			return NULL;
		}
		data_offset += current->full_size;
	}

	return image;
}

img3_element* img3_parse_element(char* data) {
	img3_element_header* element_header = (img3_element_header*) data;
	img3_element* element = (img3_element*) malloc(sizeof(img3_element));
	if (element == NULL) {
		error("ERROR: Unable to allocate memory for IMG3 element\n");
		return NULL;
	}
	memset(element, '\0', sizeof(img3_element));

	element->data = (char*) malloc(element_header->full_size);
	if (element->data == NULL) {
		error("ERROR: Unable to allocate memory for IMG3 element data\n");
		free(element);
		return NULL;
	}
	memcpy(element->data, data, element_header->full_size);
	element->header = (img3_element_header*) element->data;
	element->type = (img3_element_type) element->header->signature;

	return element;
}

void img3_free(img3_file* image) {
	if (image != NULL) {
		if (image->header != NULL) {
			free(image->header);
		}

		int i;
		for (i = 0; i < image->num_elements; i++) {
			img3_free_element(image->elements[i]);
			image->elements[i] = NULL;
		}
		free(image);
		image = NULL;
	}
}

void img3_free_element(img3_element* element) {
	if (element != NULL) {
		if (element->data != NULL) {
			free(element->data);
			element->data = NULL;
		}
		free(element);
		element = NULL;
	}
}

int img3_replace_signature(img3_file* image, char* signature) {
	int i, oldidx;
	int offset = 0;
	img3_element* ecid = img3_parse_element(&signature[offset]);
	if (ecid == NULL || ecid->type != kEcidElement) {
		error("ERROR: Unable to find ECID element in signature\n");
		return -1;
	}
	offset += ecid->header->full_size;

	img3_element* shsh = img3_parse_element(&signature[offset]);
	if (shsh == NULL || shsh->type != kShshElement) {
		error("ERROR: Unable to find SHSH element in signature\n");
		return -1;
	}
	offset += shsh->header->full_size;

	img3_element* cert = img3_parse_element(&signature[offset]);
	if (cert == NULL || cert->type != kCertElement) {
		error("ERROR: Unable to find CERT element in signature\n");
		return -1;
	}
	offset += cert->header->full_size;

	if (image->idx_ecid_element >= 0) {
		img3_free_element(image->elements[image->idx_ecid_element]);
		image->elements[image->idx_ecid_element] = ecid;
	} else {
		if (image->idx_shsh_element >= 0) {
			// move elements by 1
			oldidx = image->idx_shsh_element;
			for (i = image->num_elements-1; i >= oldidx; i--) {
				image->elements[i+1] = image->elements[i];
				switch (image->elements[i+1]->type) {
				case kShshElement:
					image->idx_shsh_element = i+1;
					break;
				case kCertElement:
					image->idx_cert_element = i+1;
					break;
				case kEcidElement:
					image->idx_ecid_element = i+1;
					break;
				default:
					break;
				}
			}
			image->elements[oldidx] = ecid;
			image->idx_ecid_element = oldidx;
			image->num_elements++;
		} else {
			// append if not found
			image->elements[image->num_elements] = ecid;
			image->idx_ecid_element = image->num_elements;
			image->num_elements++;
		}
	}

	if (image->idx_shsh_element >= 0) {
		img3_free_element(image->elements[image->idx_shsh_element]);
		image->elements[image->idx_shsh_element] = shsh;
	} else {
		if (image->idx_cert_element >= 0) {
			// move elements by 1
			oldidx = image->idx_cert_element;
			for (i = image->num_elements-1; i >= oldidx; i--) {
				image->elements[i+1] = image->elements[i];
				switch (image->elements[i+1]->type) {
				case kShshElement:
					image->idx_shsh_element = i+1;
					break;
				case kCertElement:
					image->idx_cert_element = i+1;
					break;
				case kEcidElement:
					image->idx_ecid_element = i+1;
					break;
				default:
					break;
				}
			}
			image->elements[oldidx] = shsh;
			image->idx_shsh_element = oldidx;
			image->num_elements++;
		} else {
			// append if not found
			image->elements[image->num_elements] = shsh;
			image->idx_shsh_element = image->num_elements;
			image->num_elements++;
		}

		error("%s: ERROR: no SHSH element found to be replaced\n", __func__);
		img3_free_element(shsh);
		return -1;
	}

	if (image->idx_cert_element >= 0) {
		img3_free_element(image->elements[image->idx_cert_element]);
		image->elements[image->idx_cert_element] = cert;
	} else {
		// append if not found
		image->elements[image->num_elements] = cert;
		image->idx_cert_element = image->num_elements;
		image->num_elements++;
	}

	return 0;
}

int img3_get_data(img3_file* image, char** pdata, int* psize) {
	int i;
	int offset = 0;
	int size = sizeof(img3_header);

	// Add up the size of the image first so we can allocate our memory
	for (i = 0; i < image->num_elements; i++) {
		size += image->elements[i]->header->full_size;
	}

	info("reconstructed size: %d\n", size);

	char* data = (char*) malloc(size);
	if (data == NULL) {
		error("ERROR: Unable to allocate memory for IMG3 data\n");
		return -1;
	}

	// Add data to our new header (except shsh_offset)
	img3_header* header = (img3_header*) data;
	header->full_size = size;
	header->signature = image->header->signature;
	header->data_size = size - sizeof(img3_header);
	header->image_type = image->header->image_type;
	offset += sizeof(img3_header);

	// Copy each section over to the new buffer
	for (i = 0; i < image->num_elements; i++) {
		memcpy(&data[offset], image->elements[i]->data, image->elements[i]->header->full_size);
		if (image->elements[i]->type == kShshElement) {
			header->shsh_offset = offset - sizeof(img3_header);
		}
		offset += image->elements[i]->header->full_size;
	}

	if (offset != size) {
		error("ERROR: Incorrectly sized image data\n");
		free(data);
		*pdata = 0;
		*psize = 0;
		return -1;
	}

	*pdata = data;
	*psize = size;
	return 0;
}
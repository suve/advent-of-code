#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALGO_LEN 512
#define MAX_SIZE 256

struct Image {
	int width;
	int height;
	char data[MAX_SIZE * MAX_SIZE];
	char empty;
};

#define BORDER 1

struct Image *enhance(const char *algo, const struct Image *old) {
	struct Image *new = malloc(sizeof(struct Image));

	new->width = old->width + (BORDER * 2);
	if(new->width > MAX_SIZE) {
		fprintf(stderr, "Image width exceeds maximum size of %d\n", MAX_SIZE);
		exit(EXIT_FAILURE);
	}

	new->height = old->height + (BORDER * 2);
	if(new->height > MAX_SIZE) {
		fprintf(stderr, "Image height exceeds maximum size of %d\n", MAX_SIZE);
		exit(EXIT_FAILURE);
	}

	for(int y = 0; y < new->height; ++y) {
		for(int x = 0; x < new->width; ++x) {
			int value = 0;
			for(int dy = -1; dy <= +1; ++dy) {
				int oy = (y - BORDER) + dy;
				if((oy < 0) || (oy >= old->height)) {
					value = (value * 2) + (old->empty == '#');
					value = (value * 2) + (old->empty == '#');
					value = (value * 2) + (old->empty == '#');
					continue;
				}
				
				for(int dx = -1; dx <= +1; ++dx) {
					int ox = (x - BORDER) + dx;
					if((ox < 0) || (ox >= old->width)) {
						value = (value * 2) + (old->empty == '#');
						continue;
					}
					
					int offset = (oy * old->width) + ox;
					value = (value * 2) + (old->data[offset] == '#');
				}
			}

			int offset = (y * new->width) + x;
			new->data[offset] = algo[value];
		}
	}

	// Determine whether infinite pixels in new image are lit or unlit
	new->empty = algo[old->empty == '.' ? 0 : ALGO_LEN-1];
	return new;
}

int count_pixels(const struct Image *img) {
	int count = 0;
	for(int offset = 0; offset < img->width * img->height; ++offset) {
		count += (img->data[offset] == '#');
	}
	return count;
}

void read_input(char **algo, struct Image **image) {
	char buffer[1024];
	if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
		perror("Unexpected end of input");
		exit(EXIT_FAILURE);
	}

	size_t buflen = strlen(buffer) - 1; // Remove trailing newline
	if(buflen != ALGO_LEN) {
		fprintf(
			stderr,
			"Invalid enhancement algo length: got %lu bytes, expected %d\n",
			buflen, ALGO_LEN
		);
		exit(EXIT_FAILURE);
	}

	*algo = malloc(ALGO_LEN);
	memcpy(*algo, buffer, ALGO_LEN);
	
	// Skip empty line between algo and image
	if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
		fprintf(stderr, "Unexpected end of input\n");
		exit(EXIT_FAILURE);
	}

	struct Image *img = malloc(sizeof(struct Image));
	*image = img;
	
	memset(img, 0, sizeof(struct Image));
	img->empty = '.'; // Infinite pixels of starting image are un-lit

	while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
		size_t buflen = strlen(buffer) - 1;
		if(img->width != 0) {
			if((int)buflen != img->width) {
				fprintf(
					stderr,
					"Inconsistent image width: expected %d chars, got %lu on line %d\n",
					img->width, buflen, (img->height + 1)
				);
				exit(EXIT_FAILURE);
			}
		} else {
			img->width = buflen;
		}
		memcpy(img->data + (img->width * img->height), buffer, img->width);
		img->height += 1;
	}
}

#define PART_ONE 2
#define PART_TWO 50

int main(void) {
	char *algo;
	struct Image *image;
	read_input(&algo, &image);

	for(int step = 0; step < PART_ONE; ++step) {
		struct Image *enhanced = enhance(algo, image);
		free(image);

		image = enhanced;
	}
	printf("Part1: %d\n", count_pixels(image));

	for(int step = PART_ONE; step < PART_TWO; ++step) {
		struct Image *enhanced = enhance(algo, image);
		free(image);

		image = enhanced;
	}
	printf("Part2: %d\n", count_pixels(image));

	free(image);
	free(algo);
	return 0;
}

// Written by Bruh and modified by sergray

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "MLX42/MLX42.h"
#define WIDTH 256
#define HEIGHT 256

// Exit the program as failure.
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

// Print the window width and height.
static void ft_hook(void* param)
{
	const mlx_t* mlx = param;
    (void)mlx;
	// printf("WIDTH: %d | HEIGHT: %d\n", mlx->width, mlx->height);
}

/**
 * Creates an RGBA color from the given red, green, blue, and alpha components.
 *
 * @param red   The red component of the color (0-255).
 * @param green The green component of the color (0-255).
 * @param blue  The blue component of the color (0-255).
 * @param alpha The alpha component of the color (0-255), where 0 is fully transparent and 255 is fully opaque.
 * @return      The RGBA color as a 32-bit unsigned integer.
 */
uint32_t rgba_color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    return (red << 24) | (green << 16) | (blue << 8) | alpha;
}

/**
 * Draws a line on the given image using Bresenham's line algorithm.
 *
 * @param img   The image on which the line will be drawn.
 * @param x0    The x-coordinate of the starting point of the line.
 * @param y0    The y-coordinate of the starting point of the line.
 * @param x1    The x-coordinate of the ending point of the line.
 * @param y1    The y-coordinate of the ending point of the line.
 * @param color The color of the line in RGBA format, where:
 *              - The highest 8 bits represent the red channel.
 *              - The next 8 bits represent the green channel.
 *              - The next 8 bits represent the blue channel.
 *              - The lowest 8 bits represent the alpha channel (transparency).
 *              For example, 0xFF0000FF represents a fully opaque red color.
 */
void draw_line(mlx_image_t* img, int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (x0 <= x1 && y0 <= y1)
    {
        mlx_put_pixel(img, x0, y0, color);
        int e2 = 2 * err;
        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

int32_t	main(void)
{
    const uint32_t red = rgba_color(255, 0, 0, 255);
    const uint32_t green = rgba_color(0, 255, 0, 255);

	// MLX allows you to define its core behaviour before startup.
	mlx_set_setting(MLX_MAXIMIZED, true);
	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "42", true);
	if (!mlx)
		ft_error();

	/* Do stuff */

	// Create and display the image.
	mlx_image_t* img = mlx_new_image(mlx, 512, 512);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error();

	// Even after the image is being displayed, we can still modify the buffer.
	mlx_put_pixel(img, 1, 1, rgba_color(255, 255, 0, 255));

    // Draw "4"
    draw_line(img, 20, 20, 20, 60, red); // Vertical line
    draw_line(img, 20, 60, 40, 60, red); // Horizontal line
    draw_line(img, 40, 20, 40, 80, red); // Vertical line

    // Draw "2"
    draw_line(img, 60, 20, 80, 20, green); // Top horizontal line
    draw_line(img, 80, 20, 80, 50, green); // Right vertical line
    draw_line(img, 60, 50, 80, 50, green); // Middle horizontal line
    draw_line(img, 60, 50, 60, 80, green); // Left vertical line
    draw_line(img, 60, 80, 80, 80, green); // Bottom horizontal line

	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	mlx_loop_hook(mlx, ft_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
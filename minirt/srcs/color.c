/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/04 00:50:37 by jinhyeop          #+#    #+#             */
/*   Updated: 2023/08/05 02:18:35 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

int	amb_light(t_canvas canvas, t_ray3 *ray, int idx)
{
	int	ret;

	ret = (double)ray->color[idx] * canvas.amb_bright \
		* ((double)canvas.amb_col[idx] / 255.0);
	return (ret);
}

int	diff_light(t_canvas canvas, t_ray3 *ray, double angle, int idx)
{
	int	ret;

	ret = (double)ray->color[idx] * angle * canvas.light_bright \
		* ((double)canvas.light_col[idx] / 255.0);
	return (ret);
}

int	phong_light(t_canvas canvas, t_ray3 *ray, double angle, int idx)
{
	int	ret;

	(void)ray;
	ret = (double)canvas.light_col[idx] * canvas.light_bright \
		* pow(angle, 0.95);
	// printf("%lf\n", pow((angle), (double)ray->color[idx] / 255.0));
	return (ret);
}

int	add_color(t_canvas canvas, t_ray3 *ray, double *angle, int idx)
{
	int	color;

	color = amb_light(canvas, ray, idx);
	if (ray->type != SHADOW)
		color = color + diff_light(canvas, ray, angle[0], idx) + phong_light(canvas, ray, angle[1], idx);
		// color = color + phong_light(canvas, ray, angle[1], idx);
	if (color > 255)
		color = 255;
	return (color);
}

void	ray_color(t_canvas canvas, t_ray3 *ray)
{
	double	angle[2];

	if (ray->type == SP)
	{
		angle[0] = cos_sp(ray->obj, ray, canvas);
		angle[1] = ref_sp(ray->obj, ray, canvas);
	}
	else if (ray-> type == PL)
	{
		angle[0] = cos_pl(ray->obj, ray, canvas);
		angle[1] = ref_pl(ray->obj, ray, canvas);
	}
	else
	{
		angle[0] = 0.0;
		angle[1] = 0.0;
	}
	ray->color[RED] = add_color(canvas, ray, angle, RED);
	ray->color[GREEN] = add_color(canvas, ray, angle, GREEN);
	ray->color[BLUE] = add_color(canvas, ray, angle, BLUE);
}

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seodong-gyun <seodong-gyun@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/27 02:07:30 by seodong-gyu       #+#    #+#             */
/*   Updated: 2023/08/05 02:44:30 by seodong-gyu      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minirt.h"

t_vec3	left_upper(t_camera cam, double viewport[])
{
	t_vec3	left_upper;
	t_vec3	r_half;
	t_vec3	v_half;
	t_vec3	tmp;

	r_half = multiple_vector(0.5 * viewport[WIDTH], cam.r_norm);
	v_half = multiple_vector(0.5 * viewport[HEIGHT], cam.v_norm);
	tmp = sub_vector(multiple_vector(cam.focal_len, cam.dir), v_half);
	left_upper = sub_vector(tmp, r_half);
	return (left_upper);
}

t_camera	camera(t_canvas canvas)
{
	t_camera	cam; 
	t_vec3		up;
	double		fov_radians;
	double		viewport[2];

	// 카메라 좌표
	cam.origin = canvas.cam_orig;

	// fov각도에 따른 viewport와 카메라의 거리	
	fov_radians = (double)canvas.fov * M_PI / 180.0;
	cam.focal_len = 1.0 / tan(0.5 * fov_radians);
	
	// 정규화 되어있는 viewport 가로 세로 길이
	viewport[HEIGHT] = 2.0;
	viewport[WIDTH] = viewport[HEIGHT] * canvas.ratio;

	// viewport를 향하는 direction vector
	cam.dir = canvas.cam_dir;

	up = vec3(0.0, 1.0, 0.0);
	cam.r_norm = vector_product(cam.dir, up); // right = dir x up
	cam.v_norm = vector_product(cam.dir, cam.r_norm); // down = dir x right
	cam.left_upper = left_upper(cam, viewport);
	printf("%f, %f, %f\n", cam.r_norm.x, cam.r_norm.y, cam.r_norm.z);
	printf("%f, %f, %f\n", cam.v_norm.x, cam.v_norm.y, cam.v_norm.z);
	printf("%f, %f, %f\n", cam.left_upper.x, cam.left_upper.y, cam.left_upper.z);
	return (cam);
}

t_ray3	create_ray(t_camera cam, double u, double v)
{
	t_ray3	ray;
	t_vec3	on_vp;

	ray.origin = cam.origin;
	on_vp = add_three_vector(cam.left_upper, multiple_vector(u, cam.r_norm),\
		multiple_vector(v, cam.v_norm));
	// printf("%f, %f, %f\n", on_vp.x, on_vp.y, on_vp.z);
	ray.dir = norm_vec(on_vp);
	// printf("(%f, %f, %f, size: %f)\n", ray.dir.x, ray.dir.y, ray.dir.z, size_of_vec(ray.dir.x, ray.dir.y, ray.dir.z));
	ray.t = -1.0;
	return (ray);
}

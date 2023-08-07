#include "../includes/minirt.h"
#include <stdio.h> //remove

void	hit_sphere(t_ray3 *ray, t_sphere *sp)
{
	t_vec3	l;
	double	tca;
	double	tnc;
	double	d2;
	double	tmp;


	// p = p0 + tV
	l = sub_vector(sp->center, ray->origin); // 원점과 구 중심을 잇는 방향벡터
	tca = scalar_product(l, ray->dir); // 빛이 구를 향해가고 있는지를 판단
	if (tca < 0)
		return ;
	d2 = scalar_product(l, l) - (tca * tca); // 원점에서 벡터 사이의 거리가 r^2보다 크면 FALSE d = d^2
	if (d2 > sp->radius * sp->radius)
		return ;
	tnc = sqrt(sp->radius * sp->radius - d2);
	if (tca - tnc < 0.0)
		tmp = tca + tnc;
	else
		tmp = tca - tnc;
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->color[RED] = sp->color[RED];
		ray->color[GREEN] = sp->color[GREEN];
		ray->color[BLUE] = sp->color[BLUE];
		ray->type = SP;
		ray->obj = (void *)sp;
	}
}

int	hit_line_sphere(t_ray3 *ray, t_sphere *sp)
{
	t_vec3	l;
	double	tca;
	double	d2;
	double	tnc;


	// p = p0 + tV
	l = sub_vector(sp->center, ray->origin); // 원점과 구 중심을 잇는 방향벡터
	tca = scalar_product(l, ray->dir); // 빛이 구를 향해가고 있는지를 판단
	if (tca < 0)
		return (0);
	d2 = scalar_product(l, l) - (tca * tca); // 원점에서 벡터 사이의 거리가 r^2보다 크면 FALSE d = d^2
	if (d2 > sp->radius * sp->radius)
		return (0);
	tnc = sqrt(sp->radius * sp->radius - d2);;
	if (tca - tnc < 0.0)
		return (1);
	else
		return (1);
	return (1);
}

double distance_a(t_vec3 a, t_vec3 b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    double dz = a.z - b.z;
    return sqrt(dx*dx + dy*dy + dz*dz);
}


double	intersect_sphere_shadow(t_ray3 *ray, t_canvas canvas, int num_of_light)
{
	t_vec3	p;
	t_vec3	g_norm;
	t_vec3	sh;
	t_ray3	light;
	int		idx[2];
	double	count;

	idx[0] = -1;
	count = 0;
	p = multiple_vector(ray->t, ray->dir);
	sh = p;
	while (++idx[0] < num_of_light)
	{
		idx[1] = -1;
		p.x += my_rand_double_range(-0.1, 0.1);
		p.y += my_rand_double_range(-0.1, 0.1);
		p.z += my_rand_double_range(-0.1, 0.1);
		g_norm = norm_vec(sub_vector(canvas.light_orig, p));
		light.dir = g_norm;
		light.origin = p;
		while (++idx[1] < canvas.obj->sp_cnt)
		{
			if (hit_line_sphere(&light, &canvas.obj->sp[idx[1]]))
			{
				count++;
				break ;
			}
		}
		p = sh;
	}
	if (count == 0)
		return (0.0);
	return ((double)count / num_of_light);
}

double mapToRange(double value, double minInput, double maxInput, double minOutput, double maxOutput)
{
    // 로그 함수를 사용하여 입력값을 [0, ∞) 범위로 매핑
    double logValue = log(value - minInput + 1);

    // [0, ∞) 범위의 값을 [minOutput, maxOutput] 범위로 선형 변환
    return (((logValue) / (log(maxInput - minInput + 1))) * (maxOutput - minOutput) + minOutput);
}


int	hit_plane(t_ray3 *ray, t_plane *pl, t_canvas canvas)
{
	double	tmp;
	double	scalar[3];
	double	ll;
	double	dist;
	

	(void)canvas;
	scalar[0] = scalar_product(pl->on_plane, pl->norm);
	scalar[1] = scalar_product(ray->origin, pl->norm);
	scalar[2] = scalar_product(ray->dir, pl->norm);
	tmp = (scalar[0] - scalar[1]) / scalar[2];
	if ((ray->t < 0.0 && tmp > 0.0) || (tmp > 0.0 && ray->t > tmp))
	{
		ray->t = tmp;
		ray->type = PL;
		ray->color[RED] = pl->color[RED];
		ray->color[GREEN] = pl->color[GREEN];
		ray->color[BLUE] = pl->color[BLUE];
		ray->obj = (void *)pl;
		ll = intersect_sphere_shadow(ray, canvas, 10); // 그림자 개수 --> 안티엘리어싱
		if (ll)
		{
			ray->type = SHADOW;
			dist = distance_a(ray->origin, canvas.light_orig);
			dist = mapToRange(dist, 0, 10, 1.0, 1.3);
			dist = mapToRange(dist * ll, 0, 0.8, 1.8, 1.0);
			ray->color[RED] *= dist;
			ray->color[GREEN] *= dist;
			ray->color[BLUE] *= dist;
		}
		return (1);
	}
	return (0);
}


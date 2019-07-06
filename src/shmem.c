#include "m_pd.h"

static t_class *shmem_class;

typedef struct _shmem {
	t_object  x_obj;
	t_symbol *x_arrayname;
} t_shmem;

void shmem_bang(t_shmem *x)
{
	t_garray *a;
	int npoints;
	t_word *vec;

	if (!(a = (t_garray *)pd_findbyclass(x->x_arrayname, garray_class)))
	{
		pd_error(x, "%s: no such array", x->x_arrayname->s_name);
		return;
	}

	if (!garray_getfloatwords(a, &npoints, &vec))
	{
		pd_error(x, "%s: bad template for shmem", x->x_arrayname->s_name);
		return;
	}


	post("%f", vec[0].w_float);

}

void *shmem_new(t_symbol *s)
{
	t_shmem *x = (t_shmem *)pd_new(shmem_class);
	x->x_arrayname = s;
	outlet_new(&x->x_obj, &s_float);

	return (void *)x;
}

void shmem_setup(void) {

	shmem_class = class_new(gensym("shmem"),
		(t_newmethod)shmem_new,
		0, sizeof(t_shmem),
		0, A_DEFSYM, 0);

	class_addbang(shmem_class, shmem_bang);
}


#include "m_pd.h"

static t_class *shmem_class;

typedef struct _shmem {
	t_object  x_obj;
} t_shmem;

void shmem_bang(t_shmem *x)
{
	post("Hello world !!");
}

void *shmem_new(void)
{
	t_shmem *x = (t_shmem *)pd_new(shmem_class);
	return (void *)x;
}

void shmem_setup(void) {

	shmem_class = class_new(gensym("shmem"),
		(t_newmethod)shmem_new,
		0, sizeof(t_shmem),
		CLASS_DEFAULT, 0);

	class_addbang(shmem_class, shmem_bang);
}


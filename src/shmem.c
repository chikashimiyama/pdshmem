#include <windows.h>
#include "m_pd.h"

static t_class *shmem_class;

typedef struct _shmem {
	t_object  x_obj_;
	t_symbol *arrayname_;

	DWORD size_;
	HANDLE hMapFile_;
	LPVOID pBuf_;
} t_shmem;

t_garray* findArray(t_symbol* name)
{
	return (t_garray*)pd_findbyclass(name, garray_class);
}

void shmem_bang(t_shmem *x)
{
	int npoints;
	t_word *vec;
	t_garray *array = findArray(x->arrayname_);
	if (!array)
	{
		pd_error(x, "%s: no such array", x->arrayname_->s_name);
		return;
	}

	if (!garray_getfloatwords(array, &npoints, &vec))
	{
		pd_error(x, "%s: bad template for shmem", x->arrayname_->s_name);
		return;
	}

	int pointsToProcess = garray_npoints(array);
	if (pointsToProcess < (int)x->size_)
		pointsToProcess = (int)x->size_;


	float* pBufPtr = (float*)x->pBuf_;
	for (int i = 0; i < pointsToProcess; i++)
	{
		pBufPtr[i] = vec[i].w_float;
	}
}

void *shmem_new(t_symbol *s, t_float numSamples)
{
	if (numSamples < 1.0f)
	{
		error("at lease one sample of data is required");
		return NULL;
	}

	t_shmem *x = (t_shmem *)pd_new(shmem_class);
	x->arrayname_ = s;
	x->size_ = numSamples;

	x->hMapFile_ = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, x->size_ * sizeof(float), s->s_name);
	if (x->hMapFile_ == NULL)
	{
		pd_error(x, "cannot create managed file: %s");
		return NULL;
	}

	x->pBuf_ = MapViewOfFile(x->hMapFile_, FILE_MAP_ALL_ACCESS, 0, 0, x->size_ * sizeof(float));
	if (x->pBuf_ == NULL)
	{
		CloseHandle(x->hMapFile_);
		pd_error(x, "cannot map view of file");
	}

	x->size_ = numSamples;
	return (void *)x;
}

void shmem_delete(t_shmem* x)
{
	UnmapViewOfFile(x->pBuf_);
	CloseHandle(x->hMapFile_);
}

void shmem_setup(void) {

	shmem_class = class_new(
		gensym("shmem"),
		(t_newmethod)shmem_new,
		(t_method)shmem_delete, 
		sizeof(t_shmem),
		0, A_SYMBOL, A_FLOAT, 0);

	class_addbang(shmem_class, shmem_bang);
}




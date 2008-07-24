template <class T>
void *objectRun(void *o) {
	return (void*)(((T*)o)->run());
};

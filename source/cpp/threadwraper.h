template <class T>
void *objectRun(void *o) {
	((*T)o).run();
}
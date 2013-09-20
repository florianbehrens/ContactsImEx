// (c) 2013 by Florian Behrens

#ifndef UNIQUE_PTR_H_
#define UNIQUE_PTR_H_

namespace std {

template <typename T>
class default_delete
{
public:
	void operator()(T *t) {
		delete t;
	}
};

template <typename T>
class delete_later
{
public:
	void operator()(T *t) {
		t->deleteLater();
	}
};

template<typename T, typename Deleter = default_delete<T> >
class unique_ptr {
public:
	unique_ptr() : mPtr(NULL) {}

	unique_ptr(T *t) : mPtr(t) {}

	virtual ~unique_ptr() {
		if (mPtr != NULL)
			Deleter().operator()(mPtr);
	}

	T* operator->() {
		return mPtr;
	}

private:
	T *mPtr;
};

} // namespace std

#endif // UNIQUE_PTR_H_

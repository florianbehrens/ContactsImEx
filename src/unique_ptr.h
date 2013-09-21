/*
 * (c) Copyright Florian Behrens 2013.
 *
 * This file is part of ContactsImEx.
 *
 * ContactsImEx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ContactsImEx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ContactsImEx.  If not, see <http://www.gnu.org/licenses/>.
 */

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

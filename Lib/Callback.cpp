#include "Callback.h"
#include "List.h"

namespace Callback {
	typedef List<Listener> CallbackList;
	typedef CallbackList::Iterator ListIter;
	CallbackList *root = NULL;

	CallbackList &get_cb(const Type t) {
		if (!root)
			root = new CallbackList[NumCallbacks];
		return root[t];
	}

	void attach(const Type t, Listener cb) {
		get_cb(t).Push(cb);
	}

	void trigger(const Type t, bool state) {
		CallbackList &list = get_cb(t);
		for (ListIter iter = list.Begin(); iter != list.End(); ++iter)
			iter.value()(state);
	}
}; // namespace Callback
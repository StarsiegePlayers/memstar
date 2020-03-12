#ifndef __CALLBACK_H__
#define __CALLBACK_H__

namespace Callback {
	typedef void (*Listener)(bool status);

	enum Type {
		OnStarted = 0,
		OnEndframe,
		OnOpenGL,
		OnQuit,
		OnGuiDraw,
		NumCallbacks,
	};

	void attach(const Type t, Listener cb);
	void trigger(const Type t, bool state);
}; // namespace Callback



#endif // __CALLBACK_H__

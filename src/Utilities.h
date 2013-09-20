#ifndef UTILITIES_H_
#define UTILITIES_H_

class QObject;
class QString;

class Utilities {
public:
	static void showSystemToast(QObject *parent, const QString &text);
};

#endif /* UTILITIES_H_ */

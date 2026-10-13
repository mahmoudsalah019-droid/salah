#include "SMHQStrings.h"

namespace SMHQStrings
{
	static FText T(const TCHAR* S) { return FText::FromString(S); }

	FText DoorOpen()        { return T(TEXT("اضغط E لفتح الباب")); }
	FText DoorClose()       { return T(TEXT("اضغط E لقفل الباب")); }
	FText DoorLocked()      { return T(TEXT("الباب مقفول")); }
	FText LightSwitch()     { return T(TEXT("اضغط E للنور")); }
	FText InfoPoint()       { return T(TEXT("اضغط E لعرض المشروع")); }
	FText ScreenPlayPause() { return T(TEXT("اضغط E لتشغيل أو إيقاف العرض")); }
	FText MenuTitle()       { return T(TEXT("صلاح محجوب للعمارة والتصميم")); }
	FText MenuPlaces()      { return T(TEXT("الأماكن")); }
	FText DayNight()        { return T(TEXT("نهار / ليل")); }
	FText AllLightsOn()     { return T(TEXT("تشغيل كل النور")); }
	FText AllLightsOff()    { return T(TEXT("إطفاء كل النور")); }
	FText QualityLow()      { return T(TEXT("الجودة: منخفضة")); }
	FText QualityMedium()   { return T(TEXT("الجودة: متوسطة")); }
	FText QualityHigh()     { return T(TEXT("الجودة: عالية")); }
	FText CloseMenu()       { return T(TEXT("رجوع")); }
	FText Quit()            { return T(TEXT("خروج")); }
	FText Previous()        { return T(TEXT("السابق")); }
	FText Next()            { return T(TEXT("التالي")); }
	FText PlayVideo()       { return T(TEXT("تشغيل الفيديو")); }
	FText StopVideo()       { return T(TEXT("إيقاف الفيديو")); }
	FText Close()           { return T(TEXT("إغلاق")); }
}

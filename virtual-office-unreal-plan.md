# خطة المكتب الافتراضي التفاعلي — مقر صلاح محجوب للعمارة والتصميم

**المحرك:** Unreal Engine 5.8 — **المصدر:** موديل SketchUp 2023 — **إعداد:** أكتوبر 2026 — **مراجعة 3:** 2026-10-13 (تسمية BIM بتاعة يوسف، الماكيتات MQ-xx، من غير DirectLink: إعادة بناء واستيراد كل جلسة، الزجاج بسُمك، فريق وكلاء ذكاء اصطناعي، الجهاز الجديد بعد حوالي شهرين، الإيراد تحت مليون دولار)

> **الحالة:** الخطة معتمدة للتجهيز بس. **التجربة في Unreal مش هتبدأ غير لما م. صلاح يقول "ابدأ"**. الفريق لسه بيصلّح الأبواب والإضاءة في الموديل.

> **ملحوظة عن المصادر:** كل اللينكات من التوثيق الرسمي لـ Epic (dev.epicgames.com وunrealengine.com)، وقت كتابة الخطة. أي رقم مكتوب جنبه **"يحتاج تحقق"** معناه إن لازم نراجعه على صفحة Epic أو عند المورّد قبل ما نعتمده، خصوصاً الأسعار. لو لينك اتنقل، دوّر بعنوان الصفحة في [Epic Developer Community](https://dev.epicgames.com/documentation/en-us/unreal-engine).

---

## 0. الخلاصة في صفحة واحدة

| البند | القرار |
|---|---|
| النقل من SketchUp | **Datasmith** (مش FBX)، **من غير DirectLink**: كل جلسة الموديل بيتبني بالسكربت ← تصدير `.udatasmith` ← سكربت Unreal يمسح القديم ويستورد ويربط كل حاجة **بالأسماء**. |
| التفاعل | باب، نور، تمشية، تنقّل سريع، نقط معلومات، فيديو. الوكلاء بيكتبوا المنطق **C++** وبيطلّعوا منه **Blueprints** تتظبط من غير كود (مقترح — محتاج موافقتك، 7.0). |
| الإضاءة | **Lumen** للإضاءة المرتدة والانعكاسات، و**Nanite** للتفاصيل التقيلة، و**SunSky** على موقع جدة الحقيقي. |
| الجهاز الحالي | Intel Arc 140T **مدمج** (بياخد لحد 16 جيجا من الرامات): نطاق التجربة بس، بجودة Low/Medium (**Lumen Lite**) ودقة مخفّضة. للبلوبرنت والتفاعل، مش للشكل النهائي. |
| الجهاز الجديد RTX 5080 | جودة Epic، و**Hardware Ray Tracing**، و**MegaLights** للإضاءة الليلية الكتير، وهو كمان اللي هيعمل التغليف (Packaging) ويشغّل البث. |
| التسليم | (1) برنامج ويندوز `.exe` للعميل اللي عنده جهاز كويس. (2) لينك من المتصفح بـ **Pixel Streaming** للباقي. (3) فيديو وصور 360 كاحتياطي للموبايل. |
| نبدأ بإيه | تجربة البهو والاستقبال **على الجهاز الحالي**، حوالي 3–4 أيام شغل: باب يتفتح، ونور يولّع، ونقطة معلومات. **بتبدأ لما م. صلاح يقول "ابدأ" بس.** |
| المدة | **مرحلتين:** (1) على الجهاز الحالي: التجربة والتفاعل كله للدور الأرضي، حوالي 3–4 أسابيع من يوم "ابدأ". (2) بعد ما الـ RTX 5080 يوصل (حوالي منتصف ديسمبر 2026): الشكل النهائي والتسليم، حوالي **3 أسابيع**. يعني جاهز للعميل تقريباً **أول يناير 2027**. |
| الترخيص | الإيراد تحت مليون دولار: **Unreal وTwinmotion ببلاش** (حسب شروط Epic المعلنة — **يحتاج تحقق** على صفحة الترخيص قبل التسليم). |

---

## 1. نقل الموديل من SketchUp لـ Unreal

### 1.1 Datasmith ولا FBX؟ — **Datasmith**

| | Datasmith | FBX |
|---|---|---|
| أسماء العناصر (الأبواب والإضاءة) | بتتنقل زي ما هي | بتتنقل بس ساعات بتتلخبط مع الجروبات |
| نقطة اللف (المفصّلة) | بتتنقل من محاور الـ Component | محتاجة ظبط يدوي غالباً |
| الخامات | بتتنقل بأساميها وصورها | بتتنقل ناقصة وساعات بأسماء غريبة |
| تكرار نفس العنصر (100 كرسي) | بيتنقل كنسخ من عنصر واحد → أخف بكتير | ساعات بيتحول لـ 100 عنصر منفصل |
| التحديث | تصدير ملف جديد + سكربت يعيد الاستيراد ويربط بالأسماء (قرارنا: من غير DirectLink) | تصدير جديد، بس من غير أسماء وخامات مضمونة |
| الكاميرات (Scenes) | بتتنقل | لأ |

**القرار:** Datasmith للمبنى كله. FBX نستخدمه بس لعنصر لوحده جاي من برنامج تاني (زي كرسي جاهز أو ماكيت متعمل في 3ds Max).

المصادر:
- [Datasmith Supported Software and File Types](https://dev.epicgames.com/documentation/en-us/unreal-engine/datasmith-supported-software-and-file-types)
- [Using Datasmith with SketchUp Pro](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-datasmith-with-sketchup-pro)
- [Exporting Datasmith Content from SketchUp Pro](https://dev.epicgames.com/documentation/en-us/unreal-engine/exporting-datasmith-content-from-sketchup-pro-to-unreal-engine)
- [SketchUp Pro Exporter (تحميل البلجن)](https://www.unrealengine.com/en-US/plugins/sketchup-pro) — صفحة Epic بتقول إنه بيدعم UE 5.8 وSketchUp Pro من 2019 لـ 2026. SketchUp 2023 اللي عندنا جوه المدى ده. **يحتاج تحقق** يوم 1 إن التصدير لـ `.udatasmith` من SketchUp 2023 بالبلجن بتاع 5.8 شغال، وإن أسماء الـ Instances بتوصل صح. وبما إننا مش هنستخدم DirectLink، أي شكاوى عن الـ Sync مش هتأثر علينا.
- [Datasmith Python API — DatasmithSceneElement](https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/DatasmithSceneElement) (للاستيراد بالسكربت)

### 1.2 تجهيز الموديل في SketchUp قبل التصدير (مراجعة نهائية)

القواعد بتاعتك ممتازة. دي الحاجات اللي هنتأكد منها زيادة:

1. **الباب لازم يكون Component مش Group.** ونقطة أصل الكومبوننت (المحاور الحمرا والخضرا والزرقا) تكون على محور المفصّلة بالظبط، والمحور الأزرق رأسي لفوق. ده اللي Unreal بيعتبره نقطة اللف.
   - طريقة الظبط: كليك يمين على الباب ← **Change Axes** ← حط الأصل على المفصّلة السفلية.
   - الحلق (الإطار) يفضل **برّه** كومبوننت الضلفة، علشان الحلق ثابت والضلفة بس هي اللي تلف.
   - الباب بضلفتين = كومبوننتين، كل واحد ليه مفصّلته.
   - الباب السحّاب: نفس الفكرة، والأصل في أي ركن، وهيتحرك على خط بدل ما يلف.
   - **اسم الضلفة الكامل (زي `DR_GF_E28_D43_L`) يتكتب في اسم الـ Instance** (Entity Info ← Instance)، مش بس في اسم الـ Definition. لو 10 أبواب نفس الموديل، هيبقوا Definition واحدة و10 Instances كل واحد باسمه. هنتأكد في التجربة إن Datasmith بينقل اسم الـ Instance (Check 1).
2. **وحدات الإضاءة:** نفس الكلام: Component، واسمها الثابت في اسم الـ Instance. SketchUp مفيهوش لمبات حقيقية، فاللي هيتنقل هو **شكل الوحدة** (الجسم). في Unreal هنحط جوه كل وحدة لمبة حقيقية أوتوماتيك بالاسم (شوف الملحق التقني A.4). علشان كده الاسم الثابت مهم جداً.
3. **الوشوش المقلوبة:** في SketchUp، أي وش أزرق-رمادي (Back Face) باين للناس هيطلع غلط في Unreal. استخدم **View ← Face Style ← Monochrome** وصلّح أي وش مقلوب (**Reverse Faces**).
4. **الخامة على الوش الأمامي بس**، مش على الجروب من برّه ولا على الوش الخلفي.
5. **امسح الحاجات المستخبية والزيادة:** **Window ← Model Info ← Statistics ← Purge Unused**، وامسح أي Tags مقفولة مش محتاجينها.
6. **الزجاج قطعة رفيعة بسُمك** (مش وش واحد)، وخامته اسمها فيه Glass (زي `Glass_Clear_Low-E`). ده أحسن للـ Collision (الشخص مايعديش منه). في Unreal: خامة الزجاج شفافة، وNanite مقفول عليها، وشفافية الخامة نفسها تبقى أعلى شوية علشان الطبقتين (الوش والضهر) مايغمّقوش الزجاج (بنظبطها في التجربة بالعين).
7. **قسّم الموديل بالـ Tags:** Tag لكل دور (`GF`, `L01`…`L06`, `ROOF`) وTag لكل نوع (`ARCH`, `FURN`, `DOOR`, `LIGHT`, `GLASS`). ده هيسهّل علينا نفتح دور واحد بس على الجهاز الضعيف.
8. **ملفات منفصلة لكل دور** (موصى بيه): ملف `HQ_GF.skp` للأرضي، وملف لكل دور فوق، على نفس نقطة الأصل (0,0,0). كده نقدر ننقل ونحدّث كل دور لوحده.
9. **المتر:** تمام، Datasmith بيحوّل للسنتيمتر اللي Unreal بيشتغل بيه لوحده. هنتأكد في التجربة الأولى إن ارتفاع الباب 2.10 م بيطلع 210 سم.

### 1.3 نظام التسمية: BIM بتاع يوسف (معتمد 2026-10-13، مراجعة 3)

**القاعدة:** Unreal هو اللي يتكيّف مع الموديل. الأسماء جاية من نظام يوسف، والسكربت بيفهمها.

**صيغة الباب:** `DR_GF_E28_D43_L`

| الجزء | المعنى | مثال |
|---|---|---|
| `DR` | النوع: باب | |
| `GF` | الدور | أرضي |
| `E28` | الفراغ | E-28 |
| `D43` | رقم الباب | D-43 |
| `L` / `R` | الضلفة: شمال / يمين (لو الباب ضلفة واحدة، هل في لاحقة؟ — **محتاج تأكيد**) | الشمال |

ميزة الصيغة دي: الفراغ والدور جوه الاسم نفسه، فالسكربت بيعرف لوحده الباب في أنهي أوضة، ويربط الضلفتين `L` و`R` بتوع نفس الباب علشان يفتحوا مع بعض.

| العنصر | الاسم | ملاحظة |
|---|---|---|
| ضلفة باب | `DR_GF_E28_D43_L` | Component، محاوره على المفصّلة، والاسم في الـ Instance |
| فراغ | `E28` جوه الأسماء (وE-28 في الجداول والعرض) | |
| ماكيت | `MQ-xx` | **رقم المشروع (D-07…) في الجدول بس**، مش في اسم الماكيت |
| الخامات | زي ما هي بالظبط | |
| وحدة إضاءة | **محتاج صيغة يوسف للإضاءة** — نفترض مؤقتاً إنها نفس المنطق: `LT_GF_E28_<رقم>` | السكربت بياخد الفراغ من الاسم |

**الحاجات اللي مش في الاسم بتتكتب في جداول** (Excel يتحفظ CSV)، والسكربت بيقراها:

| الجدول | الأعمدة | مثال |
|---|---|---|
| `Doors.csv` | Floor, Door, Type (Swing / Sliding), OpenAngle, OpenAngle_R, AutoOpen, Locked | `GF, D43, Swing, 90, -90, No, No` |
| `Lights.csv` | Name, Kind (DOWN / SPOT / PEND / WALL / LINE / COVE), Lumens, Kelvin, IES | `<اسم الوحدة>, DOWN, 900, 3000, <ملف IES>` |
| `Spaces.csv` | Floor, Space, NameAR, NameEN, Teleport (Yes/No), X, Y, Z, Yaw | `GF, E28, الاستقبال, Reception, Yes, 25.0, 6.0, 0.0, 90` |
| `Maquettes.csv` | Maquette, Project, Title, Description, Images, Video | `MQ-03, D-07, فيلا خاصة — أبحر, …` |
| `Materials.csv` | SketchUpMaterial, UnrealMaterial | `Marble_Calacatta, /Game/SMHQ/Materials/MI_Marble_Calacatta` |

- لو باب موجود في الموديل ومالوش سطر في `Doors.csv`، السكربت بيديله الافتراضي (لف 90°، بالزرار، مش مقفول) وبيكتبه في التقرير.
- `OpenAngle_R` للضلفة اليمين لو محتاجة اتجاه مختلف؛ لو فاضي بياخد عكس `OpenAngle`. ده بيتأكد بالعين في Check 4.
- **التعارض اللي كان بين أكواد الأبواب وأكواد المشاريع اتحل:** الأبواب `DR_…_D43_…` والماكيتات `MQ-xx`، ورقم المشروع في `Maquettes.csv` بس.
- إحداثيات نقط التنقّل في `Spaces.csv` بالمتر من نفس أصل الموديل. سكربت SketchUp يقدر يطلّعها، أو تتكتب يدوي.

### 1.4 عائلات Revit (21,490) ومكتبة SketchUp

- **ماتنقلش المكتبة كلها.** هننقل بس العناصر اللي موجودة فعلاً في الموديل.
- **الكنز الحقيقي في عائلات الإضاءة هو ملفات IES** (منحنى توزيع الضوء الحقيقي من المصنّع). ملفات `.ies` دي نطلّعها ونستخدمها في Unreal علشان النور يطلع زي الحقيقة بالظبط. [Using IES Light Profiles](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-ies-light-profiles-in-unreal-engine)
- لو محتاجين باب أو وحدة من Revit مش موجودة في SketchUp: نحطها في مشروع Revit فاضي، ونصدّرها بـ [Datasmith Exporter لـ Revit](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-datasmith-with-revit)، ونجيبها Unreal كعنصر لوحده.
- نعمل جدول بسيط (Excel) فيه: اسم العنصر في الموديل ← العائلة الأصلية ← ملف IES ← شدة الإضاءة بالـ Lumen ← درجة اللون بالكلفن. الجدول ده هيغذّي سكربت الإضاءة.

### 1.5 خطوات النقل خطوة بخطوة

**في SketchUp:**
1. حمّل **SketchUp Pro Exporter** المناسب لـ UE 5.8 من [صفحة Epic](https://www.unrealengine.com/en-US/plugins/sketchup-pro). اقفل SketchUp وشغّل الـ installer.
2. افتح SketchUp، هتلاقي **Toolbar** اسمه Datasmith.
3. اعمل **Scenes** (مشاهد) لأهم اللقطات: مدخل، استقبال، أتريوم، قاعة العرض. هتتنقل كاميرات في Unreal ونستخدمها نقط تنقّل سريع.
4. اضغط **Export** واحفظ `HQ_GF.udatasmith` في فولدر ثابت (مثلاً `D:\SMHQ\Datasmith\`). اسيب جنبه الفولدر اللي بيطلع مع الملف (فيه الصور والأشكال) ماتمسحوش.

**في Unreal 5.8:**
1. **Epic Games Launcher ← Unreal Engine ← Library ← Launch 5.8**.
2. **Games ← First Person**، واختار **Blueprint** (مش C++)، و**Starter Content** مقفول، و**Ray Tracing** مقفول على الجهاز الحالي. اسم المشروع: `SMHQ_Virtual`. (Template الـ First Person بيديك التمشية جاهزة.)
3. **Edit ← Plugins**، اتأكد إن دول شغالين (وبعدين Restart):
   - **Datasmith Importer**
   - **Datasmith Content**
   - **Sun Position Calculator** (للشمس على موقع جدة)
   - **Pixel Streaming 2** (لما نوصل للتسليم بس)
4. اعمل Level جديد: **File ← New Level ← Empty Open World** أو **Basic**، واحفظه `L_HQ_GF`.
5. من شريط الأدوات: **Quick Add (+) ← Datasmith ← File Import** واختار `HQ_GF.udatasmith`.
6. اختار فولدر الحفظ: `/Game/SMHQ/GF/`.
7. **إعدادات الاستيراد** (شوف الجدول تحت).
8. بعد الاستيراد هتلاقي في الـ Outliner عنصر اسمه **DatasmithSceneActor** وتحته كل الموديل بالأسماء.

| الإعداد في شاشة الاستيراد | القيمة | ليه |
|---|---|---|
| Geometry | ✔ | المبنى نفسه |
| Materials & Textures | ✔ | الخامات |
| Lights | ✔ | مفيش لمبات جاية من SketchUp أصلاً، بس مش هيضر |
| Cameras | ✔ | المشاهد تبقى نقط تنقّل |
| Lightmap UV — Generate Lightmap UVs | ✖ مقفول | إحنا بنستخدم Lumen، مش محتاجين Lightmaps |
| Min / Max Lightmap Resolution | سيبهم | — |
| Asset / Actor Conflict Policy | أي قيمة | السكربت بيمسح الاستيراد القديم كله قبل الجديد، فمفيش تعارض |

المرجع: [Datasmith Import Options](https://dev.epicgames.com/documentation/en-us/unreal-engine/datasmith-import-options)

9. **شغّل Nanite على الحاجات التقيلة:** في الـ Content Browser افتح `/Game/SMHQ/GF/Geometries`، اختار الأشكال المعتمة (كراسي القاعة، الماكيتات، الواجهات، الدرابزين، الحديقة) ← كليك يمين ← **Nanite ← Enable**. **ماتشغّلوش على الزجاج** (الحاجات الشفافة مش بتشتغل مع Nanite). [Nanite Virtualized Geometry](https://dev.epicgames.com/documentation/en-us/unreal-engine/nanite-virtualized-geometry-in-unreal-engine)
10. **تحقق سريع:**
    - عدّي على 3 أبواب واتأكد إن الـ Pivot (الأسهم لما تختار الباب) على المفصّلة، وإن اسمها في الـ Outliner هو اسم الـ Instance نفسه (`DR_GF_E28_D43_L`).
    - قيس باب: في نافذة جانبية (Front أو Left) اسحب بزرار الماوس الأوسط من تحت لفوق الباب، هيظهرلك المقاس. لازم 210 لو الباب 2.10 م.
    - دوّر على أي حتة سودا أو شفافة غلط = وش مقلوب ← ارجع SketchUp صلّحه.

**التحديث بعد كده: إعادة بناء كاملة كل جلسة (من غير DirectLink):**

بما إن الموديل بيتبني من الأول بالسكربت كل جلسة، الأرقام الداخلية للعناصر في SketchUp بتتغير كل مرة. فالتحديث "الذكي" مش هينفع، وممكن يعمل نسخ مكررة. الحل إن كل حاجة في Unreal تتبني من **الأسماء + الجداول**، ومفيش أي حاجة تتعمل بإيد على عناصر الموديل.

| الخطوة | فين | مين | الوقت التقريبي |
|---|---|---|---|
| 1. بناء الموديل بالسكربت | SketchUp | سكربت يوسف | دقايق |
| 2. تصدير `HQ_GF.udatasmith` (نفس المسار ونفس الاسم كل مرة) | SketchUp | يدوي، أو من السكربت لو البلجن بيسمح (**يحتاج تحقق**) | دقايق |
| 3. `smhq_rebuild.py` (الملحق A.4) | Unreal | وكيل أو زرار واحد | دقايق، حسب حجم الموديل (**يحتاج قياس في التجربة**) |
| 4. تقرير الجودة (`SMHQ_QA.csv`) | Unreal | السكربت بيكتبه | فوري |

**السكربت بيعمل إيه بالترتيب:**
1. يمسح كل اللي اتعمل في الجلسة اللي فاتت (الموديل المستورد + الأبواب والنور والنقط المتولّدة)، ويسيب الثابت: الشمس، والسما، والـ Post Process، والقوائم.
2. يستورد ملف Datasmith الجديد.
3. يحط الخامات الحقيقية مكان خامات SketchUp حسب `Materials.csv`.
4. يشغّل Nanite على الأشكال المعتمة، ويقفله على الزجاج.
5. يركّب الأبواب (ويربط الضلفتين)، واللمبات، ونقط المعلومات على الماكيتات، ونقط التنقّل.
6. يكتب تقرير: كام باب، وكام لمبة، وكام ماكيت، وأي اسم غريب أو مكرر أو ناقص من الجداول.
7. يحفظ.

**القاعدة الذهبية:** أي تظبيط (زاوية باب، شدة نور، خامة) **مكانه الجداول أو الخامات الرئيسية**، مش على العناصر جوه Unreal. اللي يتعمل على العنصر بإيد هيتمسح في إعادة البناء الجاية.

### 1.6 فين Twinmotion في الحكاية؟

Twinmotion 2026.1 ممتاز للقطات سريعة وفيديو للعميل في أيام، بس التفاعل اللي إنت طالبه (أبواب، مفاتيح نور، نقط معلومات، فيديو على شاشة، تسليم ويندوز ومتصفح) مكانه Unreal. الاقتراح:
- **Twinmotion:** صور وفيديو عرض سريع بالتوازي، من نفس ملف SketchUp.
- **Unreal:** المكتب الافتراضي التفاعلي.
- ماننقلش من Twinmotion لـ Unreal؛ ننقل من SketchUp لـ Unreal على طول، علشان يبقى عندنا مصدر واحد للحقيقة.
- **مفيد في الشهرين اللي مستنيين فيهم الجهاز الجديد:** Twinmotion أخف وممكن يطلّع صور وفيديو مبدئي للعملاء من نفس الموديل على الجهاز الحالي.

---

## 2. التفاعل بالبلوبرنت — خطوة بخطوة

### 2.0 قبل ما نبدأ: كلمتين عن البلوبرنت

- **Blueprint** = "عنصر ذكي": شكل + تصرّف. بتبنيه بتوصيل مربعات (Nodes) بخيوط، من غير كتابة كود.
- **الحدث (Event)** = "لما يحصل كذا" (مربع أحمر). **الفعل (Function)** = "اعمل كذا" (مربع أزرق).
- **المتغيّر (Variable)** = خانة بنحط فيها معلومة (مثلاً: الباب مفتوح؟ نعم/لا).
- لو علّمت على المتغيّر **Instance Editable** (العين المفتوحة)، تقدر تغيّر قيمته لكل نسخة من الـ Level من غير ما تفتح البلوبرنت. ده اللي هيخلّي حد مش مبرمج يحط 200 باب ويظبطهم من الـ Details بس.
- كل البلوبرنتس هنحطها في `/Game/SMHQ/Blueprints/`.

> **مع فريق الوكلاء:** الخطوات الجاية مكتوبة علشان أي حد في المكتب يفهم كل عنصر بيعمل إيه ويقدر يظبطه أو يعدّله. التنفيذ الفعلي المقترح: الوكلاء يكتبوا نفس المنطق بالظبط كـ **C++** (لأن الوكلاء بيكتبوا كود نصي كويس، ومش بيعرفوا يرسموا مربعات البلوبرنت بالماوس)، وبعدين يعملوا **Blueprint** من كل كلاس فيه نفس المتغيّرات (OpenAngle، AutoOpen…) ظاهرة في الـ Details. يعني إنت بتتعامل مع Blueprints عادي، والمنطق اللي جوه متكتب كود. **محتاج موافقتك** (7.0).

مراجع:
- [Blueprints Quick Start Guide](https://dev.epicgames.com/documentation/en-us/unreal-engine/quick-start-guide-for-blueprints-visual-scripting-in-unreal-engine)
- [Timelines](https://dev.epicgames.com/documentation/en-us/unreal-engine/timelines-in-unreal-engine)
- [Blueprint Interface](https://dev.epicgames.com/documentation/en-us/unreal-engine/blueprint-interface-in-unreal-engine)
- [Enhanced Input](https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine)

### 2.1 زرار "تفاعل" واحد لكل حاجة (حرف E)

هنعمل زرار واحد **E** يفتح الباب، أو يولّع النور، أو يفتح معلومات الماكيت، حسب إنت واقف قدام إيه.

1. Content Browser ← **Add ← Input ← Input Action** ← سمّيه `IA_Interact` (نوعه Digital/bool).
2. افتح الـ Input Mapping Context الموجود مع الـ First Person Template (اسمه غالباً `IMC_Default`) ← **+** ← اختار `IA_Interact` ← المفتاح **E**.
3. Content Browser ← **Add ← Blueprint ← Blueprint Interface** ← سمّيه `BPI_Interact` ← جواه دالة اسمها `Interact`.
4. افتح بلوبرنت الشخصية (`BP_FirstPersonCharacter`) ← في الـ Event Graph:
   - كليك يمين ← اكتب `IA_Interact` ← اختار الحدث **EnhancedInputAction IA_Interact** ← من **Started**:
   - **Line Trace By Channel** (خط وهمي من عين الشخص لقدّام):
     - Start = **Get World Location** للـ FirstPersonCamera
     - End = نفس النقطة + (**Get Forward Vector** للكاميرا × 250) ← يعني 2.5 متر قدامك
   - من **Out Hit ← Break Hit Result ← Hit Actor** ← **Interact (Message)** (رسالة BPI_Interact).
   - الفكرة: أي حاجة الخط خبط فيها وبتفهم "رسالة التفاعل" هتتصرّف؛ واللي مابتفهمهاش مش هيحصل حاجة.
5. (اختياري لطيف) نقطة صغيرة في نص الشاشة (Crosshair) تتلوّن لما تبص على حاجة قابلة للتفاعل.

### 2.2 الباب: يفتح ويقفل بزرار أو لما الشخص يقرب

**بلوبرنت واحد لكل الأبواب: `BP_Door`**

**أ. التكوين:**
1. **Add ← Blueprint Class ← Actor** ← `BP_Door`.
2. في **Components**:
   - `DoorPivot` (**Scene** فاضي) — ده المفصّلة.
   - تحته `DoorMesh` (**Static Mesh**) — الضلفة.
   - `ProximityBox` (**Box Collision**) — المنطقة اللي لو دخلتها الباب يفتح. حجمها مثلاً 200×300×220 سم.
3. **Class Settings ← Interfaces ← Add ← BPI_Interact**.
4. المتغيّرات (كلها Instance Editable):
   | المتغيّر | النوع | القيمة الافتراضية | معناه |
   |---|---|---|---|
   | `OpenAngle` | Float | 90 | زاوية الفتح (−90 يفتح للناحية التانية) |
   | `AutoOpen` | Boolean | ✖ | يفتح لوحده لما تقرب؟ |
   | `IsSliding` | Boolean | ✖ | سحّاب؟ |
   | `SlideDistance` | Float | 100 | مسافة السحب بالسنتيمتر |
   | `IsLocked` | Boolean | ✖ | مقفول بالمفتاح؟ (مثلاً غرف السيرفر) |
   | `IsOpen` | Boolean | ✖ | حالة الباب دلوقتي (مش Editable) |

**ب. الحركة (Timeline):**
1. Event Graph ← كليك يمين ← **Add Timeline** ← `TL_Door`.
2. افتحها ← **+ Float Track** اسمه `Alpha` ← نقطتين: (0 ثانية، 0) و(1.2 ثانية، 1). كليك يمين على النقط ← **Auto** علشان الحركة تبقى ناعمة.
3. من **Update** للتايملاين:
   - لو `IsSliding` = لأ: **Lerp (Float)** من 0 لـ `OpenAngle` بالـ Alpha ← **Make Rotator** (Yaw = الناتج) ← **Set Relative Rotation** على `DoorMesh`.
   - لو `IsSliding` = نعم: **Lerp** من 0 لـ `SlideDistance` ← **Make Vector** (X = الناتج) ← **Set Relative Location** على `DoorMesh`.

**ج. فتح/قفل بالزرار:**
1. كليك يمين ← **Event Interact** (من الـ Interface).
2. **Branch** على `IsLocked` ← لو مقفول: **Play Sound** (صوت "تك") وخلاص.
3. لو مش مقفول: **Branch** على `IsOpen`:
   - لو مقفول ← `TL_Door` **Play** ← Set `IsOpen` = نعم.
   - لو مفتوح ← `TL_Door` **Reverse** ← Set `IsOpen` = لأ.

**د. الفتح الأوتوماتيك لما يقرب:**
1. اختار `ProximityBox` ← تحت في Details ← **On Component Begin Overlap (+)**.
   - **Branch** على `AutoOpen` وبعدين اتأكد إن اللي دخل هو اللاعب (**Other Actor == Get Player Pawn**) ← **Play**.
2. **On Component End Overlap (+)** ← نفس الشروط ← **Reverse**.

**هـ. ربطه بالموديل (أهم خطوة):**
1. اسحب `BP_Door` في الـ Level.
2. في Details ← `DoorMesh` ← **Static Mesh** ← اختار شكل الضلفة اللي جه من Datasmith (`DR_GF_E28_D43_L`).
3. حط الـ BP_Door بالظبط في مكان الضلفة الأصلية. **أسهل طريقة:** اختار الضلفة الأصلية ← انسخ الـ Location والـ Rotation من الـ Details ← الصقهم في الـ BP_Door.
4. اخفي الضلفة الأصلية اللي من Datasmith (**Actor Hidden In Game ✔** و**Visible ✖**). ماتمسحهاش؛ هي جزء من الموديل المستورد، وسكربت إعادة البناء هو اللي بيمسحها ويرجعها كل جلسة.
5. علشان 200 باب: السكربت في الملحق A.4 بيعمل الخطوات دي أوتوماتيك لكل ضلفة اسمها بالصيغة `DR_<دور>_<فراغ>_<باب>_<L/R>`، وبياخد من `Doors.csv` النوع والزاوية والفتح الأوتوماتيك والقفل.
6. **الباب بضلفتين:** السكربت بيربط `L` و`R` بتوع نفس الباب (متغيّر `PairedLeaf`)، فلما تضغط E على أي ضلفة، الاتنين يفتحوا مع بعض.

**و. صوت:** نحط صوت فتح وقفل (Sound Cue) على **Play** و**Reverse**. يفرق في الإحساس جداً.

### 2.3 تشغيل وتطفية النور في كل أوضة

**الفكرة:** كل أوضة فيها **مدير نور** واحد `BP_RoomLights`، وفيها مفتاح أو أكتر على الحيطة `BP_LightSwitch` بيكلّم المدير.

**أ. `BP_RoomLights`:**
1. **Blueprint Class ← Actor** ← `BP_RoomLights`. ملوش شكل؛ بيتحط في نص الأوضة.
2. متغيّرات Instance Editable:
   | المتغيّر | النوع | معناه |
   |---|---|---|
   | `RoomCode` | Name | الدور والفراغ، مثلاً `GF_E28` |
   | `Lights` | Array of **Light** (Object Reference) | اللمبات اللي في الأوضة |
   | `EmissiveMeshes` | Array of Static Mesh Actor | أجسام الوحدات اللي بتنوّر |
   | `IsOn` | Boolean | حالة النور |
   | `StartOn` | Boolean | تبدأ والنور والع؟ |
3. دالة **SetLights (On: Boolean)**:
   - **For Each Loop** على `Lights` ← **Set Visibility** للـ Light Component = On. (Set Visibility أخف من تغيير الشدة.)
   - **For Each Loop** على `EmissiveMeshes` ← **Set Scalar Parameter Value on Materials** اسمه `EmissiveStrength` = (On ? 1 : 0). ده اللي بيخلي شكل الوحدة نفسه "يطفي" مع اللمبة.
   - Set `IsOn` = On.
4. دالة **Toggle**: تنادي SetLights بعكس `IsOn`.
5. **Event BeginPlay** ← SetLights(`StartOn`).
6. **ملء القائمة أوتوماتيك:** بدل ما تختار اللمبات واحدة واحدة، في **Construction Script**: **Get All Actors with Tag** (التاج = `RoomCode`) ← حطهم في `Lights`. والسكربت (ملحق A.4) بيدي كل لمبة تاج فراغها من اسمها نفسه (مثلاً `GF_E28`)، وكمان بيحط `BP_RoomLights` واحد لكل فراغ فيه لمبات.

**ب. `BP_LightSwitch`:**
1. **Actor** فيه Static Mesh لمفتاح على الحيطة + Interface `BPI_Interact`.
2. متغيّر Instance Editable: `TargetRoom` (Object Reference من نوع BP_RoomLights). من الـ Details تختار المدير بالـ eyedropper.
3. **Event Interact** ← `TargetRoom` ← **Toggle** ← صوت "تك".

**ج. أزرار في القائمة:**
في قائمة الشاشة (2.4) نضيف: **"كل النور"** و**"نهار/ليل"**. زرار "كل النور" يعمل **Get All Actors of Class ← BP_RoomLights ← For Each ← SetLights**.

**د. نهار وليل بزرار واحد: `BP_DayNight`:**
- بيمسك الـ SunSky (2.3/3.1) ويغيّر **Solar Time** (مثلاً 10:00 الصبح ↔ 20:00 بالليل) بتايملاين ناعم 3 ثواني، وفي نفس الوقت يولّع كل الـ BP_RoomLights لما يبقى ليل.

### 2.4 التمشية والتنقّل السريع

**أ. التمشية (جاهزة من الـ First Person Template):**
- **W A S D** للمشي، و**الماوس** للبص، و**Shift** للجري (نضيفه)، و**Space** للنط (نلغيه؛ مش مناسب لمكتب).
- **ارتفاع العين:** في `BP_FirstPersonCharacter` ← **Capsule Component** ← Half Height = 88 (يعني ارتفاع الشخص حوالي 1.76 م) وكاميرا عند حوالي 160–165 سم. نخلّيها مطابقة لعين إنسان حقيقي علشان المقاسات تتحس صح.
- **سرعة المشي:** Character Movement ← **Max Walk Speed** = 250 (2.5 م/ث تمشية مريحة في مبنى)، والجري 500.
- **السلالم:** Character Movement ← **Max Step Height** = 20 سم و**Walkable Floor Angle** = 45.
- **الأصطدام:** Datasmith بيعمل Collision للأشكال أوتوماتيك. لو الشخص بيعدّي من حيطة: افتح الشكل ← **Collision ← Add Box Simplified Collision** أو حط **Collision Complexity = Use Complex Collision As Simple** للحيطان الكبيرة. **الزجاج لازم يبقى عليه Collision** (أكتر غلطة بتتكرر).
- **الأسانسير:** في المرحلة الأولى مايتمش؛ التنقل بين الأدوار بالتنقل السريع. لاحقاً ممكن أسانسير حقيقي (نفس فكرة الباب السحاب بس الكابينة كلها بتتحرك).

**ب. التنقّل السريع (Teleport):**
1. اسحب **Target Point** في كل مكان مهم (مدخل، استقبال، أتريوم، قهوة، معرض، قاعة العرض، اجتماعات، مصلى، وكل دور لاحقاً). الاتجاه بتاعه = اتجاه البصة.
2. في الـ Details ← **Tags** ← حط اسم واضح بالعربي أو الإنجليزي: مثلاً `الاستقبال`.
3. **Widget Blueprint** اسمه `WBP_Menu` (الشاشة اللي بتظهر لما تضغط **M** أو **Tab**):
   - زرار لكل مكان، وعليه صورة صغيرة (Thumbnail) من المكان.
   - الزرار ← **Get All Actors with Tag** (التاج) ← أول عنصر ← **Get Actor Location / Rotation** ← على الشخص: **Teleport** ثم **Set Control Rotation**.
   - (أحسن) تعتيم الشاشة نص ثانية قبل وبعد النقل (**Start Camera Fade**) علشان مايدوخش.
   - تحت: أزرار **نهار/ليل**، **كل النور**، **الجودة (منخفضة/متوسطة/عالية)**، **خروج**.
4. في `BP_FirstPersonCharacter`: Input Action جديد `IA_Menu` على **M** ← يظهر/يخفي `WBP_Menu` ويحرر الماوس (**Set Input Mode UI Only / Game Only** و**Show Mouse Cursor**).
5. **خريطة الدور (مرحلة لاحقة):** صورة مسقط الدور في الـ Widget، والأماكن نقط قابلة للضغط.

مرجع: [UMG UI Designer Quick Start](https://dev.epicgames.com/documentation/en-us/unreal-engine/umg-ui-designer-quick-start-guide-in-unreal-engine)

### 2.5 نقط المعلومات (ماكيت أو مشروع ← صورة أو فيديو)

**`BP_InfoPoint` بلوبرنت واحد لكل المعروضات.** السكربت بيحط واحد فوق كل ماكيت اسمه `MQ-xx`، وبيجيب بياناته من `Maquettes.csv` (رقم المشروع، العنوان، الصور، الفيديو).

1. **Actor** فيه:
   - `Trigger` (**Sphere Collision**، نص قطر حوالي 200 سم).
   - `Marker` (**Widget Component** صغير "i" عايم فوق الماكيت ويلف ناحية الشخص: **Space = Screen**).
2. Interface: `BPI_Interact`.
3. متغيّرات Instance Editable:
   | المتغيّر | النوع | مثال |
   |---|---|---|
   | `MaquetteCode` | Name | MQ-03 |
   | `ProjectCode` | Text | D-07 (من الجدول) |
   | `Title` | Text | فيلا خاصة — أبحر |
   | `Description` | Text | سطرين: النوع، المساحة، السنة |
   | `Images` | Array of Texture 2D | صور المشروع |
   | `Video` | File Media Source | (اختياري) فيديو المشروع |
4. **لما تقرب** (Begin Overlap للاعب): يظهر تحت الشاشة "**اضغط E لعرض المشروع**".
5. **Event Interact** ← يعمل **Create Widget** من `WBP_InfoPanel` ويبعتله البيانات ← **Add to Viewport**:
   - `WBP_InfoPanel` فيه: عنوان، وصف، صورة كبيرة بأسهم يمين/شمال، وزرار "تشغيل الفيديو" لو فيه فيديو، وزرار إغلاق (أو **Esc**).
   - الفيديو جوه الـ Panel: **Image** خامته من Media Texture (نفس طريقة 2.6).
6. **لما تبعد** (End Overlap): تتقفل لوحدها.
7. **مصدر البيانات (اختياري ومفيد جداً):** بدل ما نكتب بيانات كل مشروع في الـ Details، نعمل **Data Table** من `Maquettes.csv` (الماكيت MQ-xx ← المشروع D-01…D-24، والاسم، والوصف، والصور). ونقطة المعلومات تسحب بياناتها بالكود. ساعتها تحديث المعرض = تعديل الإكسل وإعادة استيراده. [Data Driven Gameplay Elements](https://dev.epicgames.com/documentation/en-us/unreal-engine/data-driven-gameplay-elements-in-unreal-engine)

### 2.6 شاشة قاعة العرض تشغّل فيديو

المرجع الأساسي: [Play a Video File in Unreal Engine](https://dev.epicgames.com/documentation/en-us/unreal-engine/play-a-video-file-in-unreal-engine) و[Media Framework](https://dev.epicgames.com/documentation/en-us/unreal-engine/media-framework-in-unreal-engine).

1. **جهّز الفيديو:** `MP4`، كوديك **H.264**، 1920×1080، 30 fps، 10–15 Mbps. (H.265 و4K ممكن بس اتأكد على جهاز العميل.)
2. **حطه في `Content/Movies/`** جوه فولدر المشروع على الويندوز. **ده مهم جداً:** أي فيديو برّه الفولدر ده مش هيتغلّف مع البرنامج وهيطلع شاشة سودا عند العميل.
3. Content Browser ← **Add ← Media ← File Media Source** ← `MS_Showreel` ← اختار ملف الفيديو.
4. **Add ← Media ← Media Player** ← `MP_Auditorium` ← علّم **Video output Media Texture asset** ← هيعمل `T_MP_Auditorium` لوحده.
5. **الخامة:** Material جديد `M_Screen`:
   - Texture Sample (الـ Media Texture) ← **Emissive Color** (مضروب في Scalar اسمه `Brightness` = 2 تقريباً).
   - **Base Color** = أسود، **Roughness** = 0.3.
   - كده الشاشة بتنوّر وبتأثر على القاعة كلها بالإضاءة بفضل Lumen (نور الفيديو بيقع على الكراسي — شكله حلو جداً في قاعة مضلمة).
6. حط `M_Screen` على شكل الشاشة في قاعة العرض.
7. **`BP_CinemaScreen`** (Actor):
   - متغيّرات: `Player` (MP_Auditorium)، `Source` (MS_Showreel)، `AutoPlayOnEnter` (✔).
   - Component: **Media Sound** (صوت الفيديو، مربوط بالـ Media Player) متحط عند الشاشة؛ و`HallTrigger` (Box Collision) قد القاعة.
   - دخول القاعة ← **Open Source** (Source) ← (الفيديو بيشتغل لوحده لو **Play on Open** ✔) ← ونطفّي نور القاعة لـ 10% (BP_RoomLights).
   - خروج من القاعة ← **Pause** ← ونرجّع النور.
   - **Event Interact** على الشاشة ← تشغيل/إيقاف.
   - **Loop** ✔ في الـ Media Player.
8. **الصوت 3D:** في Media Sound ← **Attenuation** ← قطر حوالي 30 م، علشان الصوت يوطى وإنت خارج من القاعة.

---

## 3. الإضاءة والخامات والواقعية

### 3.1 الإضاءة الطبيعية (النهار)

1. **SunSky** (من بلجن Sun Position Calculator): **Quick Add ← Lights ← SunSky** (أو من Place Actors). [Sun and Sky Actor](https://dev.epicgames.com/documentation/en-us/unreal-engine/sun-and-sky-actor-in-unreal-engine)
2. في الـ Details:
   - **Latitude** ≈ 21.49 (شمال)، **Longitude** ≈ 39.19 (شرق) — جدة. **يحتاج تحقق** بإحداثيات الموقع الافتراضي لو له قطعة أرض محددة.
   - **Time Zone** = +3.
   - **North Offset**: لفّه لحد ما يطابق اتجاه الشمال في المسقط.
   - **Date/Time**: نعمل لقطات افتراضية مثلاً 21 مارس 10:00 و21 ديسمبر 15:30.
3. الـ SunSky بيجيب معاه **Directional Light** و**Sky Light** و**Sky Atmosphere**. خلّي الـ Sky Light على **Real Time Capture ✔** علشان الأتريوم يتنوّر صح مع أي وقت.
4. **الأتريوم والسكاي لايت:** ده أهم مكان بصرياً. Lumen هيعمل النور المرتد من الحديقة على الحيطان لوحده. اتأكد إن زجاج السقف خامته شفافة فعلاً، وإن مفيش أي سقف معتم مستخبي تحته، وإلا الشمس مش هتدخل. الزجاج بسُمك فيه طبقتين، فخلي بالك إن الشفافية الإجمالية ماتبقاش غامقة.
5. **التعريض (Exposure):** حط **Post Process Volume** ← **Infinite Extent (Unbound) ✔**:
   - **Exposure ← Metering Mode = Manual** للتحكم الكامل، أو Auto بحدود ضيقة (Min/Max EV100 مثلاً 6–10 للنهار جوه).
   - **ماتسيبش Auto Exposure مفتوح على الآخر**، علشان الداخل بيبان منوّر زيادة والخارج محروق.
6. **وحدات حقيقية:** شغّل [Physical Lighting Units](https://dev.epicgames.com/documentation/en-us/unreal-engine/physical-lighting-units-in-unreal-engine): الشمس حوالي 75,000–120,000 lux، واللمبات بالـ Lumen من كتالوج المصنّع.

### 3.2 الإضاءة الليلية

1. **BP_DayNight** ينزّل الشمس تحت الأفق (Solar Time مثلاً 20:00) ويولّع كل الأوض.
2. **كل لمبة:**
   - النوع: Spot للداون لايت والسبوت، **Rect Light** للخطي والكوف، Point للمعلّق.
   - **IES Profile** من ملفات المصنّع (Revit) ← في Light Profiles ← **IES Texture** ✔ **Use IES Brightness**.
   - **Temperature** ✔: 3000K للاستقبال والقهوة، 4000K للمكاتب، و2700K للمصلى (اقتراح للمراجعة).
   - **Source Radius** صغير بدل صفر، علشان الظل يبقى ناعم واقعي.
   - **Cast Shadows**: خليه للمهم بس على الجهاز الحالي.
3. **MegaLights** (production-ready في 5.8 حسب إعلان Epic): بيسمح بمئات اللمبات اللي بتعمل ظل من غير ما الجهاز يقع. **على الجهاز الجديد بس.** **Project Settings ← Rendering ← MegaLights ✔**. [MegaLights](https://dev.epicgames.com/documentation/en-us/unreal-engine/megalights-in-unreal-engine)
4. **شكل الوحدة نفسها:** خامة Emissive للجزء اللي بينوّر (تتحكم فيها BP_RoomLights).
5. **الإضاءة الخارجية للواجهة والحديقة:** Up-lights على الشجر و Wall-washers على الواجهة؛ بتفرق جداً في لقطة الليل من برّه.

### 3.3 Lumen

مرجع: [Lumen Global Illumination and Reflections](https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-global-illumination-and-reflections-in-unreal-engine) و[Lumen Performance Guide](https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-performance-guide-for-unreal-engine).

- **Project Settings ← Rendering:**
  - **Dynamic Global Illumination Method = Lumen**
  - **Reflection Method = Lumen**
  - **Shadow Map Method = Virtual Shadow Maps**
  - **Support Hardware Ray Tracing** = ✖ على الجهاز الحالي، ✔ على الـ RTX 5080.
- **Lumen Lite** (جديد في 5.8): نسخة أخف وأسرع تقريباً للضعف، وبتشتغل على **مستوى جودة Medium** للـ Global Illumination حسب ما اتنشر عن الإصدار. ده اللي هنستخدمه على الجهاز الحالي وكمان للعملاء اللي أجهزتهم متوسطة. **يحتاج تحقق** من طريقة التفعيل بالظبط في [Release Notes 5.8](https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-5-8-release-notes).
- **حيطان رفيعة = تسريب نور:** Lumen بيحب الحيطان اللي سمكها 10 سم أو أكتر. أي سقف أو حيطة "وش واحد" من SketchUp ممكن النور يعدّي منها. صلّحها في SketchUp (Push/Pull) أو حط بلوك أسود ورا.
- **الأوض المقفولة الكبيرة (قاعة العرض):** لو فيها بقع أو "غليان" (Noise): Post Process ← **Lumen Global Illumination ← Final Gather Quality** = 2 على الجهاز الجديد.

### 3.4 Nanite

- شغّله على كل حاجة معتمة فيها تفاصيل (اتشرح في 1.5 خطوة 9).
- **ماتشغّلوش** على الزجاج والحاجات الشفافة.
- **الشجر والنباتات (الحديقة والخارج):** من **Fab** (متجر Epic: [fab.com](https://www.fab.com)) نختار شجر ونخيل متوافق مع Nanite. نحطه في Unreal مش في SketchUp زي ما اتفقنا. **يحتاج تحقق** من ترخيص كل أصل (مجاني ولا مدفوع) وقت الاختيار.
- مرجع: [Nanite Virtualized Geometry](https://dev.epicgames.com/documentation/en-us/unreal-engine/nanite-virtualized-geometry-in-unreal-engine)

### 3.5 الخامات

**المبدأ:** خامة رئيسية (Master Material) واحدة لكل عيلة، والباقي نسخ (Material Instances) بتتظبط من غير برمجة.

| الخامة الرئيسية | تستخدم لـ | الإعدادات اللي هتتظبط |
|---|---|---|
| `M_Master_Opaque` | حوائط، أرضيات، خشب، حجر، معدن، قماش | Base Color / Normal / Roughness / Metallic + مقياس حقيقي بالمتر |
| `M_Master_Glass` | زجاج الواجهة والفواصل والسكاي لايت (قطع بسُمك) | لون خفيف، Roughness ≈ 0.05، شفافية مظبوطة على طبقتين |
| `M_Master_Emissive` | وحدات الإضاءة والشاشات | شدة الإضاءة، اللون |
| `M_Master_Water` | (لو فيه مية في الحديقة) | — |
| `M_Master_Foliage` | نباتات جوه الأتريوم | Two Sided Foliage |

الخطوات:
1. Datasmith هيجيب الخامات بأساميها الحقيقية. اعمل جدول (Excel): اسم الخامة في SketchUp ← الخامة الرئيسية ← خامة Fab/Megascans المقابلة ← المقاس الحقيقي للبلاطة/اللوح.
2. استبدل الخامات المهمة بخامات PBR حقيقية عالية الجودة (رخام، خشب، حجر، خرسانة) من Fab. **Material Replace**: في Content Browser كليك يمين على الخامة ← **Asset Actions ← Replace References**، أو بسكربت بالاسم.
3. **المقياس:** أكتر حاجة بتبوّظ الواقعية إن البلاطة تطلع 3 م بدل 60 سم. في الخامة الرئيسية نحط باراميتر `TileSize_m` ونخلّي الـ UV بالمتر الحقيقي (World Aligned) للحوائط والأرضيات الكبيرة.
4. **Roughness Variation:** طبقة خفيفة من البقع والاختلاف في اللمعة على الأرضيات والرخام. بتفرق جداً.
5. **الحواف:** حواف الأثاث والحوائط الحادة بزيادة بتبان "CG". لو ينفع، Bevel صغير 2–5 مم في SketchUp للحاجات القريبة من العين (الاستقبال، الطرابيزات).

### 3.6 قايمة "إزاي الشكل يبقى حقيقي"

1. مقياس صح (ارتفاع العين، المقاسات، مقياس الخامات).
2. خامات PBR حقيقية + Roughness مش ثابت.
3. إضاءة بوحدات فيزيائية وIES حقيقي.
4. تعريض ثابت ومظبوط (مش Auto على الآخر).
5. تفاصيل الحياة: كتب، ماكيتات، أقلام، فناجين، نباتات، شاشات شغالة، لوحات على الحيطة. المكتب الفاضي بيبان افتراضي.
6. Post Process خفيف: **Bloom** قليل، **Vignette** 0.2، **Film Grain** صفر أو قليل جداً، **Lens Flare** مقفول.
7. **Reflections:** على الجهاز الجديد شغّل Hardware Ray Tracing علشان الزجاج والرخام اللامع يعكس صح.
8. صوت المكان: صوت خفيف للنافورة/الحديقة، صدى في الأتريوم، ماكينة القهوة. الصوت نص الواقعية.

---

## 4. الأداء: الجهاز الحالي والجهاز الجديد

### 4.1 الجهاز الحالي (Core Ultra 7 255H + Intel Arc 140T مدمج + 31 جيجا متاحة)

**الصورة بصراحة:** الكارت **مدمج** وبياخد من الرامات لحد 16 جيجا. يعني اللي فاضل لويندوز وUnreal والبرامج التانية حوالي 15–16 جيجا. ده أقل بكتير من توصية Epic العامة لـ UE5 (كارت DirectX 12 بذاكرة 8 جيجا أو أكتر، و32 جيجا رامات). [Hardware and Software Specifications](https://dev.epicgames.com/documentation/en-us/unreal-engine/hardware-and-software-specifications-for-unreal-engine) — **يحتاج تحقق** على صفحة 5.8.

**دوره في المشروع (لحد ما الجهاز الجديد يوصل بعد حوالي شهرين):** التجربة الأولى، وبعدها **كل التفاعل والسكربتات للدور الأرضي كله** بجودة Low. **الشكل النهائي** للإضاءة والخامات والتسليم بيستنوا الجهاز الجديد.

**ملحوظة عن الوكلاء:** الجهاز ده يقدر يشغّل نسخة **واحدة** بس من Unreal في المرة. يعني الوكلاء يقدروا يكتبوا كود وسكربتات وجداول بالتوازي، بس الاستيراد والتجربة والتغليف بيمشوا **واحد ورا التاني** على الجهاز ده.

**الإعدادات:**
1. اللابتوب **على الشاحن** دايماً، و**Windows ← Power Mode ← Best performance**، وفي **Intel Graphics Software** اختار وضع الأداء. وحدّث درايفر Intel Arc لآخر نسخة.
2. **ماتفتحش SketchUp وTwinmotion وUnreal مع بعض.** الرامات هي عنق الزجاجة.
3. نطاق التجربة بس في الـ Level. باقي الدور في ملف منفصل.
4. نافذة الشغل: **Settings ← Engine Scalability Settings ← Low** وإنت بتبني، و**Medium** (Lumen Lite) وإنت بتقيس أو بتقيّم.
5. **Screen Percentage** 50% في نافذة الشغل.
6. **Realtime** في النافذة اقفله (Ctrl+R) وإنت مش محتاج تشوف حركة.
7. **Hardware Ray Tracing** مقفول، و**MegaLights** مقفول، وعدد قليل من اللمبات اللي بتعمل ظل.
8. حجم الصور: أقصى حاجة 2K للخامات في التجربة.
9. **Editor Preferences ← General ← Performance ← Use Less CPU when in Background ✔**.
10. **أول فتحة للمشروع هتاخد وقت طويل** (Compiling Shaders). لو الجهاز علّق أو الرامات خلصت أثناءها، سيبه يكمّل، ولو اتكررت نقلل عدد العمليات اللي شغالة على التوازي (ملحق A.7).
11. المشروع والكاش على الـ SSD الداخلي.

**Check 11** (30 fps أو أكتر على 1080p بجودة Medium **على الجهاز الحالي**) هو أكتر Check ممكن يفشل بسبب الكارت المدمج. **مش هنسهّله**؛ هنقيسه بأمانة. لو فشل، هنكتبه "فاشل" ومعاه الرقم الحقيقي وأسبابه، وإنت تقرر: إما استثناء صريح، أو نعيد القياس على الجهاز الجديد.

### 4.2 الجهاز الجديد (RTX 5080 + 64 جيجا)

| البند | الحالي | الجديد |
|---|---|---|
| جودة الشغل | Low/Medium (كارت مدمج) | **Epic** |
| Lumen | Lite / Software | **Hardware Ray Tracing** |
| انعكاسات الزجاج والرخام | تقريبية | دقيقة |
| اللمبات الليلية بظل | قليلة | مئات بـ **MegaLights** |
| أدوار مفتوحة مع بعض | الأرضي بس | المبنى كله بـ **World Partition** و**Level Streaming** |
| تحسين الأداء | TSR | TSR أو **DLSS** (بلجن NVIDIA — **يحتاج تحقق** من توفره لـ 5.8) |
| التغليف (Packaging) | بطيء | سريع |
| Pixel Streaming | لأ | **أيوه**، يقدر يبقى السيرفر نفسه لـ 1–2 مستخدم في نفس الوقت |
| صور وفيديو عالي الجودة | — | [Movie Render Queue](https://dev.epicgames.com/documentation/en-us/unreal-engine/movie-render-pipeline-in-unreal-engine) |

نصايح للجهاز الجديد:
- اختار نسخة RTX 5080 بـ **16 جيجا** VRAM (النسخة القياسية) وSSD NVMe 2 تيرا على الأقل للمشروع والكاش.
- مزود كهربا كويس وتبريد كويس، لأن Pixel Streaming معناه الجهاز شغال ساعات طويلة.
- **قبل ما نغيّر للجهاز الجديد، كل الشغل يبقى على Git أو نسخة احتياطية** (اقتراح: Perforce أو Git LFS، أو على الأقل نسخة يومية على هارد خارجي).

### 4.3 المبنى كله (أرضي + 6 أدوار + ملحق)

- الأرضي 50×40 = 2,000 م²، وكل دور فوق 50×36 = 1,800 م² × 6 = 10,800 م²، يعني حوالي **12,800 م² + الملحق**.
- **مش هنفتح المبنى كله مرة واحدة:** كل دور **Level** لوحده، و**Level Streaming**: الدور اللي إنت فيه بيتحمّل كامل، والباقي يتشال أو يبقى شكل خارجي بس. [World Partition](https://dev.epicgames.com/documentation/en-us/unreal-engine/world-partition-in-unreal-engine) و[Level Streaming](https://dev.epicgames.com/documentation/en-us/unreal-engine/level-streaming-in-unreal-engine)
- الأدوار اللي فوق **لسه متصممتش**، فهتتضاف لما تصميمها يخلص، بنفس قواعد التصدير والبلوبرنتس (من غير ما نعيد حاجة).

---

## 5. التسليم للعميل والتكلفة

### 5.1 الطرق التلاتة

| | برنامج ويندوز (.exe) | لينك من المتصفح (Pixel Streaming) | فيديو + صور 360 |
|---|---|---|---|
| العميل محتاج إيه | جهاز ويندوز بكارت قوي (8 جيجا VRAM فأكتر) | متصفح + نت كويس (حوالي 10–20 Mbps — **يحتاج تحقق**) | أي حاجة، حتى الموبايل |
| الجودة | عالية حسب جهازه | عالية (بتتعرض من جهازنا أو السيرفر) | الأعلى (متعمل مسبقاً) |
| التفاعل | كامل | كامل | لأ / مشاهدة بس |
| حجم التحميل | كبير (غالباً 5–20 جيجا — **يحتاج تحقق** بعد أول تغليف) | ولا حاجة | صغير |
| مناسب لـ | عرض في المكتب، عملاء كبار، معارض | مشاركة سريعة مع أي حد | السوشيال، الموبايل، الأرشيف |
| تكلفة تشغيل | صفر | بالساعة/بالشهر | صفر |

**الاقتراح:** نعمل التلاتة من نفس المشروع:
1. **ويندوز** للعرض في المكتب وعلى لابتوب العرض.
2. **Pixel Streaming** للعملاء برّه.
3. **فيديو جولة + صور 360** للسوشيال والموبايل.

### 5.2 برنامج ويندوز خطوة بخطوة

مرجع: [Packaging Unreal Engine Projects](https://dev.epicgames.com/documentation/en-us/unreal-engine/packaging-unreal-engine-projects)

1. **Project Settings ← Maps & Modes ← Game Default Map** = `L_HQ_GF` (أو شاشة بداية فيها لوجو المكتب واختيار نهار/ليل).
2. **Project Settings ← Description:** اسم المشروع، الشركة "Salah Mahjoub — Architecture & Design"، أيقونة.
3. **Project Settings ← Packaging:**
   - **Build Configuration = Shipping**.
   - **Use Pak File ✔**، **Use Io Store ✔**.
   - **Additional Asset Directories to Cook / Non-Asset Directories to Package**: اتأكد إن `Movies` موجود.
   - **Include Prerequisites installer ✔** (علشان يشتغل على أي ويندوز).
4. **Platforms (قايمة فوق) ← Windows ← Package Project** ← اختار فولدر.
5. الناتج فولدر فيه `SMHQ_Virtual.exe`. اعمل له ملف مضغوط أو Installer.
6. **قايمة إعدادات للعميل** (في WBP_Menu): منخفض/متوسط/عالي ← **Get Game User Settings ← Set Overall Scalability Level ← Apply Settings**. وابدأ على **Medium** افتراضياً.
7. **جرّب على جهاز تاني غير جهاز الشغل** قبل ما تبعته لأي عميل.

### 5.3 Pixel Streaming (لينك من المتصفح)

مراجع: [Pixel Streaming](https://dev.epicgames.com/documentation/en-us/unreal-engine/pixel-streaming-in-unreal-engine) و[Why Pixel Streaming 2?](https://dev.epicgames.com/documentation/en-us/unreal-engine/pixel-streaming-2-overview-in-unreal-engine) و[Pixel Streaming Infrastructure على GitHub](https://github.com/EpicGamesExt/PixelStreamingInfrastructure) (محتاج حساب GitHub مربوط بحساب Epic — **يحتاج تحقق** من مكان الريبو الحالي).

**الفكرة ببساطة:** البرنامج شغال على جهاز قوي عندنا أو في السحابة، والعميل بيشوف "فيديو حي" منه في المتصفح، والماوس والكيبورد بتوعه بيتبعتوا للجهاز. يعني جهاز العميل مش فارق.

**3 اختيارات:**

| | الطريقة | التكلفة التقريبية (**كلها تحتاج تحقق** وقت التنفيذ) | مناسب لـ |
|---|---|---|---|
| أ | **الجهاز الجديد في المكتب** هو السيرفر | الكهربا + نت بـ Upload كويس + IP ثابت/خدمة TURN | عرض لعميل واحد في نفس الوقت، بجدول مواعيد |
| ب | **سيرفر سحابي GPU** (AWS / Azure / Google Cloud) | تقريباً 0.5–1.5 دولار للساعة للسيرفر الواحد وهو شغال | لو عندنا ناس تقنيين يظبطوه |
| ج | **خدمة جاهزة** (أمثلة: Vagon Streams، Eagle 3D Streaming، Arcware) | اشتراك شهري، غالباً من عشرات لمئات الدولارات حسب الساعات والمستخدمين | **الموصى بيه**: ترفع البرنامج وتاخد لينك، من غير سيرفرات |

**الاقتراح:** نبدأ بـ **(أ)** للتجربة والعروض المجدولة (مجاني تقريباً)، ونحوّل لـ **(ج)** لما يبقى عندنا عملاء كتير عايزين يدخلوا لوحدهم في أي وقت. **(ب)** بس لو حجم الاستخدام كبير جداً.

**خطوات (أ) باختصار** (التفاصيل في الملحق A.6):
1. شغّل بلجن **Pixel Streaming 2** وغلّف البرنامج عادي.
2. نزّل **Pixel Streaming Infrastructure** (الفرع المطابق لـ 5.8) وشغّل الـ Signalling Server.
3. شغّل البرنامج بإضافة `-PixelStreamingURL=ws://127.0.0.1:8888 -RenderOffscreen`.
4. افتح المتصفح على `http://127.0.0.1` ← جرّب.
5. علشان حد برّه المكتب يدخل: محتاجين **TURN Server** وفتح بورتات أو خدمة نفق آمنة. ده جزء تقني نعمله مع حد IT مرة واحدة.

### 5.4 الترخيص

حسب إعلان Epic عن الأسعار ([Epic blog — pricing update](https://www.unrealengine.com/en-US/blog/we-are-updating-unreal-engine-twinmotion-and-realitycapture-pricing-in-late-april)):
- **Unreal مجاني** للشركات اللي **إيرادها السنوي أقل من مليون دولار** (≈ 3.75 مليون ريال).
- لو الإيراد أكتر: **Unreal Subscription** حوالي **1,850 دولار/سنة للمقعد** (≈ 6,940 ريال) — سعر 2024، **يحتاج تحقق**.
- **تغيير مهم:** Epic بتقول إنه **من 27 مايو 2026** اشتراك Unreal **ماعادش بيشمل Twinmotion**. يعني لو إيراد المكتب فوق المليون، Twinmotion ليه ترخيص منفصل (كان 445 دولار/سنة في 2024 — **يحتاج تحقق**).
- **المكتب إيراده تحت مليون دولار ← Unreal ببلاش، وTwinmotion كمان ببلاش** (Epic أعلنت إن Twinmotion مجاني للشركات اللي تحت المليون، ومعاه الاستخدام التجاري، ما عدا Twinmotion Cloud). **يحتاج تحقق** على صفحة الترخيص الرسمية قبل التسليم، لأن الأرقام دي من إعلان 2024 ومن مواقع تانية بتنقل عنه.
- **خلّي بالك:** لو الإيراد عدّى المليون في سنة، الشروط بتتغير.
- المراجعة النهائية: [Unreal Engine License / EULA](https://www.unrealengine.com/en-US/license).

### 5.5 ملخص التكلفة (تقديري — بالدولار)

| البند | مرة واحدة | سنوي/شهري | ملاحظة |
|---|---|---|---|
| ترخيص Unreal | — | **0** | الإيراد تحت مليون دولار — **يحتاج تحقق** |
| ترخيص Twinmotion | — | **0** | تحت المليون (Twinmotion Cloud مش مشمول) — **يحتاج تحقق** |
| اشتراك أدوات الوكلاء | — | حسب الخطة اللي عندكم | مش محسوب هنا |
| أصول Fab (شجر، خامات، أثاث تكميلي، أصوات) | 0–1,000 | — | كتير منها مجاني؛ نختار حسب الترخيص |
| الجهاز الجديد | (اشتريته) | — | — |
| Pixel Streaming من المكتب | — | ~0 + نت | محتاج Upload كويس |
| Pixel Streaming خدمة جاهزة | — | عشرات–مئات/شهر | **يحتاج تحقق** بعرض سعر |
| سيرفر سحابي | — | ~0.5–1.5/ساعة تشغيل | **يحتاج تحقق** |
| استضافة الـ .exe للتحميل (Google Drive / OneDrive) | — | موجود غالباً | — |

---

## 6. التجربة الأولى: البهو والاستقبال (على الجهاز الحالي — بتبدأ لما م. صلاح يقول "ابدأ" بس)

### 6.1 النطاق

- **المكان:** المدخل + الاستقبال + جزء من الأتريوم اللي باين من الاستقبال.
- **التفاعل:**
  1. باب واحد (باب المدخل أو باب جانبي) يفتح ويقفل بـ **E** و/أو لما تقرب.
  2. مجموعة نور الاستقبال تولّع وتطفي من مفتاح على الحيطة.
  3. نقطة معلومات واحدة على ماكيت (مشروع واحد بصورتين).
  4. زرار نهار/ليل.
  5. نقطتين تنقّل سريع (المدخل والاستقبال).
- **برّه النطاق:** باقي الدور، الأسانسير، الفيديو، Pixel Streaming.

### 6.2 Checks معتمدة للتجربة الأولى (اتقفلت 2026-10-13)

م. صلاح وافق على الـ 13 Check دول. **مقفولين:** مفيش تعديل ولا تسهيل أثناء الشغل غير بموافقة صريحة جديدة. (نسخة منهم محفوظة في `PROJECT.md`.)

| # | المعيار (نعم/لا) |
|---|---|
| 1 | الموديل اتنقل بـ Datasmith، وأسماء الأبواب والإضاءة والخامات في Unreal هي نفسها اللي في SketchUp. |
| 2 | المقاس صح: باب 2.10 م يتقاس 210 سم في Unreal (±1 سم). |
| 3 | مفيش وشوش مقلوبة أو خامات ناقصة (بقع سودا/شطرنج) في نطاق التجربة. |
| 4 | الباب بيلف حول المفصّلة بالظبط، ومفيش فراغ أو تداخل مع الحلق وهو بيفتح أو بيقفل. |
| 5 | الباب بيفتح بـ E من مسافة 2.5 م أو أقل، و(لو مفعّل) بيفتح لوحده لما تقرب ويقفل لما تبعد. |
| 6 | مفتاح الحيطة بيولّع ويطفي **كل** لمبات الاستقبال، وشكل الوحدات نفسه بيطفي معاها. |
| 7 | زرار نهار/ليل بيغيّر الشمس والنور الداخلي، والصورة مش محروقة ولا ضلمة زيادة في الحالتين. |
| 8 | الشخص مايعديش من أي حيطة أو زجاج، ويطلع أي سلّمة/عتبة في النطاق. |
| 9 | نقطة المعلومات بتظهر "اضغط E" لما تقرب، وبتفتح العنوان والصورتين، وبتتقفل لما تبعد أو بـ Esc. |
| 10 | التنقّل السريع بينقل للمكانين وبيبص في الاتجاه الصح. |
| 11 | على الجهاز الحالي: 30 fps أو أكتر على 1080p بجودة Medium جوه نطاق التجربة. |
| 12 | تحديث بسيط في SketchUp (تغيير خامة الأرضية) بيوصل Unreal بـ DirectLink **من غير ما الباب أو النور يبوظوا**. |
| 13 | نسخة ويندوز متغلّفة (.exe) من التجربة بتشتغل على جهاز تاني. |

> **⚠ Check 12 محتاج قرار منك:** الصيغة المعتمدة بتقول "بيوصل Unreal بـ **DirectLink**"، وإنت قررت مانستخدمش DirectLink. يعني Check 12 بصيغته الحالية مش هينفع يتقاس، وهيتكتب "يحتاج تحقق". **مش هغيّره من نفسي.** الصيغة البديلة المقترحة (مش أسهل، بالعكس أشد):
>
> **12 (مقترح):** "تعديل بسيط في SketchUp (تغيير خامة الأرضية) ← إعادة بناء الموديل بالسكربت ← تصدير Datasmith جديد ← تشغيل `smhq_rebuild.py`: التعديل بيظهر، والأبواب والنور ونقطة المعلومات شغالين زي قبلها بالظبط، ومفيش ولا عنصر مكرر أو ناقص (حسب تقرير الجودة)."
>
> لو وافقت صراحةً، هتتسجل في `PROJECT.md` بتاريخها مكان القديمة.

**الحاجات اللي الـ Checks دي مابتقيسهاش** (هنعلّق عليها في التقرير): الإحساس العام بالواقعية، توازن ألوان الخامات، راحة سرعة المشي. دي تقييمك إنت بالعين.

### 6.3 قبل التجربة: تعديلات الموديل (عند فريقك — شغال دلوقتي)

1. الأبواب ووحدات الإضاءة Components، ومحاور الأبواب على المفصّلة.
2. الأسماء في الـ Instances بنظام يوسف: `DR_GF_E28_D43_L`، والماكيتات `MQ-xx`، وصيغة الإضاءة.
3. الزجاج قطع بسُمك.
4. الجداول (`Doors`، `Lights`، `Spaces`، `Maquettes`، `Materials`) لنطاق التجربة على الأقل.
5. ملف Datasmith لنطاق التجربة بس (`HQ_GF_Pilot.udatasmith`).

**بعدها م. صلاح يراجع ويقول "ابدأ"، وساعتها بس نفتح Unreal.**

### 6.4 التجربة (على الجهاز الحالي، حوالي 3–4 أيام شغل)

الوكلاء بيجهّزوا الكود والسكربتات **بالتوازي قبل ما نفتح Unreal**، وبعدين الجهاز بيشتغل خطوة ورا خطوة:

| المرحلة | بالتوازي (وكلاء — كود وبيانات) | على الجهاز (بالترتيب) | Checks |
|---|---|---|---|
| تجهيز (نص يوم) | كلاسات C++ (الباب، النور، المفتاح، نهار/ليل، نقطة المعلومات، الشاشة، التنقّل)، `smhq_rebuild.py`، تحويل الجداول | تثبيت البلجن، إنشاء المشروع، أول تجميع للكود وللـ Shaders (ممكن ساعات) | — |
| يوم 1 | مراجعة تقرير الجودة وتصليح السكربت | `smhq_rebuild.py` على ملف التجربة، Collision، ارتفاع العين، SunSky، Exposure | 1، 2، 3، 8 |
| يوم 2 | تصليح اللي ظهر | الأبواب، والنور والمفاتيح، ونهار وليل، ونقطة المعلومات، والقايمة. **تصميم شاشتين القايمة ونقطة المعلومات محتاج حد يرصّهم بالماوس** (نص يوم) | 4، 5، 6، 7، 9، 10 |
| يوم 3 | تحضير تقرير التقييم | قياس الأداء، إعادة البناء من أول وجديد (Check 12)، تغليف `.exe` وتجربته على جهاز تاني | 11، 12، 13 |
| يوم 4 | التقرير: جدول الـ 13 Check (ناجح / فاشل / يحتاج تحقق) + لقطات شاشة + فيديو قصير | مراجعة معاك | — |

**إيه اللي محتاج إنسان:** موافقتك ومراجعتك بالعين، تثبيت البرامج من الـ Launcher (محتاج تسجيل دخول)، رصّ الشاشتين، تجربة النسخة على جهاز تاني، وتحميل أي أصول من Fab.

## 7. الجدول الزمني والمخاطر

### 7.0 فريق الوكلاء: بيعملوا إيه، وإيه اللي بيحدّهم

| الوكلاء بيعملوه كويس وبالتوازي | اللي مش بيتسرّع بزيادة الوكلاء |
|---|---|
| كود C++ للتفاعل، وسكربتات Python لـ Unreal، وسكربتات SketchUp | **جهاز واحد ضعيف**: نسخة Unreal واحدة في المرة؛ الاستيراد والتجميع والتجربة والتغليف بيمشوا بالترتيب |
| الجداول (أبواب، إضاءة، فراغات، ماكيتات، خامات) والتقارير | أول تجميع للـ Shaders (ساعات، مرة على كل جهاز) |
| مراجعة تقارير الجودة ولقطات الشاشة، وتصليح الأخطاء | **مراجعتك وموافقتك** بالعين على الشكل |
| التوثيق ودليل الاستخدام للعميل | رصّ الشاشات (القايمة ونقطة المعلومات)، وتحميل الأصول من Fab، والتثبيت من الـ Launcher |
| | وصول الجهاز الجديد |

**قواعد شغل الوكلاء:**
1. **الوكلاء لازم يشتغلوا على جهاز الويندوز اللي عليه Unreal** (مثلاً Claude Code مثبّت عليه)، علشان يشغّلوا Unreal بسطر أوامر ويقروا التقارير. الجلسة دي (اللي أنا فيها) شغالة في السحابة على Linux ومش هتقدر تشغّل Unreal على جهازك.
2. **ملفات Unreal اللي مش نصية** (الـ Levels والـ Blueprints) مايشتغلش عليها أكتر من وكيل في نفس الوقت. التوازي يبقى في الكود والسكربتات والجداول (ملفات نصية).
3. كل شغل الوكلاء يتحفظ على Git، وكل تعديل ليه تقرير.
4. **القرار المطلوب منك:** المنطق يتكتب **C++ + Blueprints للتظبيط** (المقترح، مناسب للوكلاء)، ولا **Blueprints بس** متبنية بإيد حسب القسم 2 (أبطأ، ومحتاجة حد في المكتب يرسمها). C++ محتاج **Visual Studio 2022** على الجهاز (**يحتاج تحقق** من النسخة المطلوبة لـ 5.8).

### 7.1 الجدول الزمني (فريق وكلاء + الجهاز الجديد بعد حوالي شهرين)

**الافتراضات:** النهارده 2026-10-13. الـ RTX 5080 يوصل حوالي **منتصف ديسمبر 2026**. الشغل أحد–خميس. التجربة بتبدأ يوم ما تقول "ابدأ" (اسمه **يوم A** تحت).

**المرحلة 1: على الجهاز الحالي (من يوم A لحد ما الجهاز الجديد يوصل)**

| الفترة | الشغل | المخرجات |
|---|---|---|
| يوم A → A+4 أيام | التجربة الأولى (6.4) | تقرير الـ 13 Check |
| **نقطة قرار** | مراجعة معاك | نكمل ولا نعدّل الطريقة |
| أسبوع 2–3 بعد A | الدور الأرضي كامل بجودة Low: كل الأبواب والنور والمفاتيح، كل الماكيتات MQ-xx، شاشة قاعة العرض بالفيديو، القايمة الكاملة ونقط التنقّل، إعادة البناء من أول وجديد بتشتغل في كل جلسة | نسخة تفاعلية "رمادي" للدور الأرضي + تقرير جودة نضيف |
| أسبوع 4 بعد A | تجهيز الخامات الرئيسية و`Materials.csv`، اختيار الشجر والأصول من Fab، تجهيز ملفات IES، سكربتات Pixel Streaming والتغليف | كل حاجة جاهزة تتنقل للجهاز الجديد |
| الوقت اللي فاضل لحد ما الجهاز يوصل | الأدوار اللي فوق لو تصميمها خلص (نفس السكربتات)، وصور وفيديو مبدئي من Twinmotion للعملاء | — |

> علشان المرحلة 1 تخلص قبل الجهاز الجديد، **"ابدأ" لازم تيجي خلال حوالي 4 أسابيع من النهارده** (يعني لحد منتصف نوفمبر تقريباً). لو اتأخرت أكتر، الجهاز الجديد هيستنانا بدل ما إحنا نستناه.

**المرحلة 2: بعد وصول الجهاز الجديد (حوالي 3 أسابيع)**

| الفترة | الشغل | المخرجات |
|---|---|---|
| أول 2–3 أيام | تثبيت Unreal وVisual Studio، سحب المشروع من Git، أول تجميع للـ Shaders، تشغيل Hardware Ray Tracing وMegaLights | المشروع شغال على الجهاز الجديد |
| باقي أسبوع 1 | الإضاءة النهائية نهار وليل لكل الفراغات (الأتريوم، قاعة العرض، المعرض)، الخامات النهائية، الشجر | جولتين مراجعة معاك |
| أسبوع 2 | تغليف `.exe`، Pixel Streaming من المكتب، فيديو جولة وصور 360 | نسخة تسليم أولى |
| أسبوع 3 | تجربة مع 3–5 أشخاص من برّه + تصليح | **جاهز للعميل** |

**الإجمالي:** لو "ابدأ" جت قبل منتصف نوفمبر والجهاز وصل منتصف ديسمبر ← **جاهز للعميل تقريباً أول أسبوعين في يناير 2027**. الحاجة اللي بتحدد الميعاد هي **وصول الجهاز الجديد**، مش عدد الوكلاء.

**الأدوار اللي فوق:** لما تصميمها يخلص، الدور المكرر حوالي 2–3 أيام (نفس السكربتات والجداول)، والدور المختلف حوالي أسبوع (تقدير — **يحتاج تحقق** بعد الدور الأرضي).

### 7.2 المخاطر

| # | الخطر | الاحتمال | التأثير | الحل |
|---|---|---|---|---|
| 1 | الكارت المدمج (Arc 140T) مايوصلش لـ 30 fps في التجربة (Check 11)، أو الرامات تخلص | عالي | متوسط | نطاق التجربة بس، Low/Medium، والبرامج التانية مقفولة. **Check 11 مش بيتسهّل**: لو فشل يتكتب فاشل وإنت تقرر |
| 2 | الجهاز الجديد يتأخر عن منتصف ديسمبر | متوسط | **عالي جداً** (هو اللي بيحدد ميعاد التسليم) | كل التفاعل والسكربتات يخلصوا على الحالي قبل ما يوصل؛ كل أسبوع تأخير = أسبوع على التسليم |
| 3 | الأبواب نقطة لفها غلط (Group بدل Component، أو محاور غلط) | متوسط | متوسط | مراجعة في SketchUp قبل التصدير + Check 4 في التجربة |
| 4 | إعادة البناء كل جلسة بتعمل نسخ مكررة أو بتمسح تظبيطات اتعملت بإيد | عالي لو مفيش نظام | عالي | السكربت بيمسح القديم كله قبل الاستيراد؛ **كل التظبيطات في الجداول**، مش على العناصر؛ تقرير بالمكرر والناقص |
| 5 | بلجن Datasmith بتاع 5.8 مايصدّرش صح من SketchUp 2023 | منخفض–متوسط | عالي | نتأكد يوم 1؛ البديل نسخة SketchUp أحدث، وأسوأ حالة FBX (مع خسارة جزء من الأسماء والخامات) |
| 6 | تسريب نور (حيطان وش واحد) أو Noise في Lumen | عالي | متوسط | حيطان بسُمك في SketchUp؛ Final Gather أعلى على الجديد |
| 7 | الفيديو مش بيشتغل عند العميل (شاشة سودا) | متوسط | متوسط | الفيديو في `Content/Movies`، H.264، تجربة على جهاز تاني قبل التسليم |
| 8 | جهاز العميل ضعيف للـ .exe | عالي | متوسط | قايمة جودة + Lumen Lite افتراضي + بديل Pixel Streaming |
| 9 | Pixel Streaming من المكتب ببطء أو مايفتحش برّه الشبكة | متوسط | متوسط | TURN Server؛ أو خدمة جاهزة |
| 10 | شروط الترخيص المجاني تتغير، أو الإيراد يعدّي المليون | منخفض | منخفض–متوسط | نراجع صفحة الترخيص قبل كل تسليم |
| 11 | حجم الملفات وضياع شغل | متوسط | عالي | نسخ احتياطي يومي + نظام إصدارات (Git LFS / Perforce) |
| 12 | الأدوار اللي فوق لسه متصممتش، والعميل يتوقع المبنى كله | متوسط | متوسط | نوضح من الأول إن النسخة الأولى = الأرضي + الواجهة الخارجية كاملة؛ والأدوار تتضاف تباعاً |
| 13 | ترخيص الأصول (شجر، أثاث، أصوات) مش مسموح بيه في التوزيع | منخفض | عالي | جدول بكل أصل ومصدره وترخيصه |
| 14 | 5.8 آخر إصدار رئيسي في UE5 حسب Epic، وUE6 جاي | منخفض | منخفض | نثبت المشروع على 5.8 لحد التسليم؛ ماننقلش لإصدار جديد في نص الشغل |
| 15 | اسم مكتوب غلط أو مش على الصيغة (`DR_GF_E28_D43_L`) | متوسط | متوسط | السكربت بيرفض أي اسم مش على الصيغة وبيكتبه في التقرير؛ مايخمّنش |
| 16 | Datasmith بينقل اسم الـ Definition بدل اسم الـ Instance، أو بيضيف لاحقة (`DR_GF_E28_D43_L_2`) | متوسط | متوسط | التقرير بيكشفها من أول تشغيل؛ ده جزء من Check 1 |
| 17 | جولات المراجعة معاك بتبقى عنق الزجاجة | متوسط | متوسط | ميعاد مراجعة ثابت مرتين في الأسبوع، وكل مراجعة معاها لقطات شاشة وتقرير جاهز |
| 18 | الوكلاء بيعدّلوا نفس ملف Unreal في نفس الوقت ويبوّظوه | متوسط | عالي | ملفات الـ Levels والـ Blueprints ليها وكيل واحد بس؛ التوازي في الملفات النصية؛ Git |
| 19 | الوكلاء مايقدروش يحكموا على الشكل لوحدهم | عالي | متوسط | الوكلاء بيطلّعوا لقطات وتقارير؛ الحكم النهائي على الواقعية ليك |
| 20 | الجهاز الحالي مايقدرش يستحمل الدور الأرضي كله حتى بجودة Low | متوسط | متوسط | نقسم الدور لأجزاء ونستورد الجزء اللي بنجرّبه بس |

---

## 8. اللي اتقرر واللي لسه مستني

**اتقرر (2026-10-13):**
- التسمية بنظام يوسف (BIM): `DR_GF_E28_D43_L`، والماكيتات `MQ-xx`، ورقم المشروع في الجدول بس، والخامات زي ما هي.
- الـ 13 Check بتوع التجربة: معتمدين ومقفولين (Check 12 مستني قرارك، شوف 6.2).
- من غير DirectLink: الموديل بيتبني بالسكربت كل جلسة، وبيتصدّر Datasmith، وبيتربط بالأسماء.
- الزجاج قطع بسُمك.
- الجهاز الحالي: Core Ultra 7 255H + Arc 140T مدمج. الجديد RTX 5080 بعد حوالي شهرين.
- الإيراد تحت مليون دولار: Unreal وTwinmotion ببلاش (يحتاج تحقق على صفحة الترخيص).
- الفريق: وكلاء ذكاء اصطناعي شغالين بالتوازي.
- **التجربة في Unreal مابتبدأش غير لما م. صلاح يقول "ابدأ".**

**لسه محتاج منك:**
1. **Check 12:** توافق على الصيغة البديلة (من غير DirectLink)، ولا تسيبه زي ما هو؟
2. **صيغة يوسف لأسماء الإضاءة** (ومفاتيح الحيطة لو موجودة في الموديل).
3. **الباب بضلفة واحدة:** اسمه بيخلص بـ `_L` برضه، ولا من غير لاحقة؟
4. **C++ + Blueprints** (المقترح للوكلاء) ولا **Blueprints بس**؟
5. الوكلاء هيشتغلوا على جهاز الويندوز نفسه؟ (لازم علشان يشغّلوا Unreal.)
6. فيديو الـ Showreel، وصور ونصوص المشاريع، وجدول الماكيتات MQ-xx ← المشاريع.

## A.1 Project setup

| Item | Value |
|---|---|
| Template | Games → First Person, **C++** (recommended for agent-authored logic; Blueprint children for tuning), Starter Content off |
| Project name | `SMHQ_Virtual` |
| Required plugins | Datasmith Importer, Datasmith Content, Sun Position Calculator, Python Editor Script Plugin, Editor Scripting Utilities, Pixel Streaming 2 (delivery), Media Player (built in) |
| Folder layout | `/Game/SMHQ/{GF,L01..L06,ROOF}/`, `/Game/SMHQ/Blueprints/`, `/Game/SMHQ/Materials/Masters/`, `/Game/SMHQ/UI/`, `/Game/SMHQ/Data/`, `/Content/Movies/` |
| Maps | `L_HQ_Persistent` (hand-authored, never touched by rebuild: SunSky, PPV, player start, UI managers) + per-floor sublevels `L_GF_Import` (Datasmith output, wiped every rebuild) and `L_GF_Generated` (doors/lights/info points/teleports, wiped every rebuild) |
| Rebuild entry point | `Content/Python/smhq_rebuild.py`, headless: `UnrealEditor-Cmd.exe SMHQ_Virtual.uproject -run=pythonscript -script="<abs path>/smhq_rebuild.py"` *(verify that level load/save works in commandlet mode in 5.8; otherwise run inside the editor via `-ExecutePythonScript=`)* |
| Agent constraints | One editor instance on the current laptop; binary assets (`.umap`, `.uasset`) single-owner; parallel work only on text sources (C++, Python, CSV) |
| Source control | Git + Git LFS; commit `.uasset/.umap`, ignore `Saved/`, `Intermediate/`, `DerivedDataCache/`, `Binaries/`; never commit the imported `L_GF_Import` content if it is fully reproducible (optional) |

### Project Settings → Rendering

| Setting | Current PC (Arc 140T iGPU) | New PC (RTX 5080) |
|---|---|---|
| Dynamic Global Illumination Method | Lumen | Lumen |
| Reflection Method | Lumen | Lumen |
| Shadow Map Method | Virtual Shadow Maps | Virtual Shadow Maps |
| Support Hardware Ray Tracing | Off | On (requires SM6 / DX12) |
| Use Hardware Ray Tracing when available (Lumen) | Off | On |
| MegaLights | Off | On |
| Anti-Aliasing | TSR | TSR (or DLSS plugin, *verify 5.8 availability*) |
| Default RHI | DirectX 12 | DirectX 12 |
| Target shader model | SM6 | SM6 |

### Scalability / console variables (useful)

```
stat fps
stat unit
stat gpu
r.ScreenPercentage 67
sg.GlobalIlluminationQuality 2     ; Medium -> Lumen Lite path in 5.8 (verify)
sg.ReflectionQuality 2
sg.ShadowQuality 2
r.Lumen.HardwareRayTracing 0|1
r.MegaLights.Enable 0|1           ; (verify cvar name in 5.8)
t.MaxFPS 60
r.VSync 0
```

## A.2 SketchUp export checklist

- Doors = **Components**; component axes origin on hinge line, blue (Z) axis up; frame outside the leaf component.
- Sliding doors: origin at a leaf corner; slide axis = component red (X) axis.
- One material per face, front faces only; no back faces visible (`Face Style → Monochrome` check).
- Glass = thin solid (has thickness; gives proper collision), material name contains `Glass`; tune opacity for two translucent layers.
- Walls/slabs with real thickness (≥ 10 cm) to avoid Lumen light leaks.
- Tags: floor (`GF`, `L01`…`L06`, `ROOF`) + category (`ARCH`, `FURN`, `DOOR`, `LIGHT`, `GLASS`).
- Purge unused; meters; shared origin (0,0,0) across per-floor files.
- Scenes for key viewpoints → become CineCameraActors → reusable as teleport anchors.

Naming (approved 2026-10-13, BIM scheme): `DR_<Floor>_<Space>_<Door>_<Leaf>` e.g. `DR_GF_E28_D43_L` (Leaf `L|R`; single-leaf suffix **pending**). Maquettes `MQ-xx` (project code only in `Maquettes.csv`). Lights: **pattern pending** — script assumes `LT_<Floor>_<Space>_<...>`. Materials unchanged. Names live on the **component instance** (Entity Info → Instance).

CSV schedules under `<Project>/SMHQ_Data/`:

| File | Columns |
|---|---|
| `Doors.csv` | `Floor,Door,Type,OpenAngle,OpenAngle_R,AutoOpen,Locked` (Type = Swing \| Sliding; booleans Yes/No; missing row → defaults + QA warning) |
| `Lights.csv` | `Name,Kind,Lumens,Kelvin,IES` (Kind = DOWN \| SPOT \| PEND \| WALL \| LINE \| COVE; IES = asset path or empty) |
| `Spaces.csv` | `Floor,Space,NameAR,NameEN,Teleport,X,Y,Z,Yaw` (metres, model origin) |
| `Maquettes.csv` | `Maquette,Project,Title,Description,Images,Video` → `DT_Maquettes` |
| `Materials.csv` | `SketchUpMaterial,UnrealMaterial` |

## A.3 Datasmith import options

| Option | Value |
|---|---|
| Geometry / Materials & Textures / Cameras | On |
| Lights | On (SketchUp has none; harmless) |
| Generate Lightmap UVs | Off (Lumen) |
| Asset / Actor Conflict Policy | Irrelevant — rebuild deletes previous import first (no DirectLink, IDs change every SketchUp rebuild) |
| Post-import | Enable Nanite on opaque high-poly meshes (Content Browser → right-click → Nanite → Enable). Never on translucent. |
| DirectLink | **Not used** |

Rule: **nothing is hand-edited on imported or generated actors.** Every tweak lives in CSV schedules, master materials, or class defaults; the rebuild is fully reproducible.

## A.4 Rebuild pipeline: `smhq_rebuild.py`

Run once per session after exporting a fresh `.udatasmith`. Requires Datasmith Importer, Python Editor Script Plugin, Editor Scripting Utilities. API names flagged *(verify)* must be confirmed against the 5.8 Python API on day 1 of the pilot.

```python
# Content/Python/smhq_rebuild.py — Unreal Engine 5.8, editor-only
import csv, os, re
import unreal

PROJECT   = unreal.Paths.project_dir()
DATA      = os.path.join(PROJECT, "SMHQ_Data")
UDS_FILE  = os.path.join(DATA, "export", "HQ_GF.udatasmith")
IMPORT_TO = "/Game/SMHQ/Import/GF"
GEN_TAG   = unreal.Name("SMHQ_GEN")       # everything this script creates
IMP_TAG   = unreal.Name("SMHQ_IMPORT")    # everything Datasmith imported

DOOR_RE  = re.compile(r"^DR_(?P<floor>[A-Z0-9]+)_(?P<space>E\d+)_(?P<door>D\d+)(?:_(?P<leaf>[LR]))?$")
LIGHT_RE = re.compile(r"^LT_(?P<floor>[A-Z0-9]+)_(?P<space>E\d+)_")      # pattern pending (Yusuf)
MQ_RE    = re.compile(r"(?P<mq>MQ-\d+)")

BP = lambda name: unreal.EditorAssetLibrary.load_blueprint_class(f"/Game/SMHQ/Blueprints/{name}")
DOOR_BP, ROOM_BP, INFO_BP = BP("BP_Door"), BP("BP_RoomLights"), BP("BP_InfoPoint")
LIGHT_CLASS = {"DOWN": unreal.SpotLight, "SPOT": unreal.SpotLight, "PEND": unreal.PointLight,
               "WALL": unreal.PointLight, "LINE": unreal.RectLight, "COVE": unreal.RectLight}

actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
qa = []                                    # rows: (severity, kind, name, message)

def rows(name, *key):
    path = os.path.join(DATA, name)
    if not os.path.exists(path):
        qa.append(("WARN", "data", name, "missing CSV")); return {}
    with open(path, newline="", encoding="utf-8-sig") as f:
        return {tuple(r[k].strip() for k in key): r for r in csv.DictReader(f)}

def yes(v): return str(v or "").strip().lower() in ("yes", "y", "true", "1")
def label_of(a): return re.sub(r"_\d+$", "", a.get_actor_label())        # drop Datasmith dup suffix
def mesh_comp(a): return a.get_component_by_class(unreal.StaticMeshComponent)
def tag(a, *t): a.tags = list(a.tags) + [unreal.Name(x) if isinstance(x, str) else x for x in t]
def spawn(cls, loc, rot=unreal.Rotator()):
    a = actors.spawn_actor_from_class(cls, loc, rot); tag(a, GEN_TAG); return a

# ---------------------------------------------------------------- 1. wipe
old = [a for a in actors.get_all_level_actors() if GEN_TAG in a.tags or IMP_TAG in a.tags]
actors.destroy_actors(old)
if unreal.EditorAssetLibrary.does_directory_exist(IMPORT_TO):
    unreal.EditorAssetLibrary.delete_directory(IMPORT_TO)

# ---------------------------------------------------------------- 2. import
before = set(a.get_path_name() for a in actors.get_all_level_actors())
scene = unreal.DatasmithSceneElement.construct_datasmith_scene_from_file(UDS_FILE)
scene.import_scene(IMPORT_TO)              # (verify) name in 5.8: import_scene / import_scenes
imported = [a for a in actors.get_all_level_actors() if a.get_path_name() not in before]
for a in imported:
    tag(a, IMP_TAG)

# ---------------------------------------------------------------- 3. materials + Nanite
mat_map = {k[0].lower(): r["UnrealMaterial"] for k, r in rows("Materials.csv", "SketchUpMaterial").items()}
seen_meshes = set()
for a in imported:
    c = mesh_comp(a)
    if not c or not c.static_mesh:
        continue
    is_glass = False
    for i in range(c.get_num_materials()):
        m = c.get_material(i)
        name = m.get_name().lower() if m else ""
        is_glass |= "glass" in name
        for su_name, ue_path in mat_map.items():
            if su_name.replace(" ", "_") in name:          # Datasmith may sanitise names
                c.set_material(i, unreal.EditorAssetLibrary.load_asset(ue_path))
    sm = c.static_mesh
    if sm.get_path_name() not in seen_meshes and not is_glass:
        ns = sm.get_editor_property("nanite_settings")      # (verify) property name in 5.8
        ns.enabled = True
        sm.set_editor_property("nanite_settings", ns)
        seen_meshes.add(sm.get_path_name())

# ---------------------------------------------------------------- 4. doors
door_rows = rows("Doors.csv", "Floor", "Door")
leaves = {}                                                 # (floor, door) -> [BP_Door, ...]
for a in imported:
    m = DOOR_RE.match(label_of(a))
    if not m:
        if label_of(a).startswith("DR_"):
            qa.append(("ERROR", "door", label_of(a), "name does not match DR_<Floor>_<E##>_<D##>_<L|R>"))
        continue
    c = mesh_comp(a)
    if not c:
        continue
    f, d, leaf = m["floor"], m["door"], m["leaf"] or "L"
    r = door_rows.get((f, d))
    if r is None:
        qa.append(("WARN", "door", label_of(a), "no row in Doors.csv — defaults used"))
        r = {"Type": "Swing", "OpenAngle": "90", "OpenAngle_R": "", "AutoOpen": "No", "Locked": "No"}
    angle = float(r["OpenAngle"] or 90)
    if leaf == "R":
        angle = float(r["OpenAngle_R"]) if r.get("OpenAngle_R") else -angle
    door = spawn(DOOR_BP, a.get_actor_location(), a.get_actor_rotation())
    door.set_actor_label(f"Door_{label_of(a)}")
    door.set_actor_scale3d(a.get_actor_scale3d())
    door.get_editor_property("DoorMesh").set_static_mesh(c.static_mesh)
    door.set_editor_property("OpenAngle", angle)
    door.set_editor_property("IsSliding", r["Type"].strip().lower() == "sliding")
    door.set_editor_property("AutoOpen", yes(r["AutoOpen"]))
    door.set_editor_property("IsLocked", yes(r["Locked"]))
    tag(door, f"{f}_{m['space']}")
    a.set_actor_hidden_in_game(True); a.set_is_temporarily_hidden_in_editor(True)
    leaves.setdefault((f, d), []).append(door)
for key, pair in leaves.items():
    if len(pair) == 2:
        pair[0].set_editor_property("PairedLeaf", pair[1])
        pair[1].set_editor_property("PairedLeaf", pair[0])
    elif len(pair) > 2:
        qa.append(("ERROR", "door", "_".join(key), f"{len(pair)} leaves with the same door code"))

# ---------------------------------------------------------------- 5. lights + room managers
light_rows, rooms = rows("Lights.csv", "Name"), {}
for a in imported:
    name = label_of(a)
    m = LIGHT_RE.match(name)
    if not m:
        continue
    r = light_rows.get((name,), {})
    if not r:
        qa.append(("WARN", "light", name, "no row in Lights.csv — defaults used"))
    cls = LIGHT_CLASS.get((r.get("Kind") or "DOWN").upper(), unreal.SpotLight)
    origin, extent = a.get_actor_bounds(False)
    loc = unreal.Vector(origin.x, origin.y, origin.z - extent.z - 2.0)
    rot = unreal.Rotator(0.0, -90.0, 0.0) if cls is not unreal.PointLight else unreal.Rotator()  # (roll, pitch, yaw)
    lt = spawn(cls, loc, rot)
    lt.set_actor_label(f"L_{name}")
    lc = lt.light_component
    lc.set_editor_property("intensity_units", unreal.LightUnits.LUMENS)
    lc.set_editor_property("intensity", float(r.get("Lumens") or 800))
    lc.set_editor_property("use_temperature", True)
    lc.set_editor_property("temperature", float(r.get("Kelvin") or 3000))
    if r.get("IES"):
        ies = unreal.EditorAssetLibrary.load_asset(r["IES"].strip())
        if ies:
            lc.set_editor_property("ies_texture", ies); lc.set_editor_property("use_ies_brightness", True)
    room = f"{m['floor']}_{m['space']}"
    tag(lt, room); rooms.setdefault(room, origin)
for room, origin in rooms.items():
    mgr = spawn(ROOM_BP, origin)
    mgr.set_actor_label(f"Room_{room}")
    mgr.set_editor_property("RoomCode", unreal.Name(room))

# ---------------------------------------------------------------- 6. maquette info points
mq_rows = rows("Maquettes.csv", "Maquette")
for a in imported:
    m = MQ_RE.search(label_of(a))
    if not m:
        continue
    origin, extent = a.get_actor_bounds(False)
    ip = spawn(INFO_BP, unreal.Vector(origin.x, origin.y, origin.z + extent.z + 30.0))
    ip.set_actor_label(f"Info_{m['mq']}")
    ip.set_editor_property("MaquetteCode", unreal.Name(m["mq"]))   # BP reads DT_Maquettes row
    if (m["mq"],) not in mq_rows:
        qa.append(("WARN", "maquette", m["mq"], "no row in Maquettes.csv"))

# ---------------------------------------------------------------- 7. teleport anchors
for (f, sp), r in rows("Spaces.csv", "Floor", "Space").items():
    if yes(r["Teleport"]):
        loc = unreal.Vector(float(r["X"]) * 100, float(r["Y"]) * 100, float(r["Z"]) * 100)   # m -> cm
        tp = spawn(unreal.TargetPoint, loc, unreal.Rotator(0.0, 0.0, float(r["Yaw"] or 0)))
        tp.set_actor_label(f"TP_{f}_{sp}")
        tag(tp, "Teleport", f"{f}_{sp}")

# ---------------------------------------------------------------- 8. QA + save
counts = {k: sum(1 for x in actors.get_all_level_actors() if x.get_actor_label().startswith(k))
          for k in ("Door_", "L_", "Room_", "Info_", "TP_")}
with open(os.path.join(PROJECT, "Saved", "SMHQ_QA.csv"), "w", newline="", encoding="utf-8") as f:
    w = csv.writer(f); w.writerow(["severity", "kind", "name", "message"])
    w.writerows(qa); w.writerow(["INFO", "counts", "", str(counts)])
unreal.log(f"SMHQ rebuild done: {counts}; {len(qa)} QA issues -> Saved/SMHQ_QA.csv")
unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
```

Notes:
- Doors, lights, info points and teleports are spawned in whatever level is current; set `L_GF_Generated` as current level before running (or load it via `EditorLevelLibrary`/`LevelEditorSubsystem`).
- `ERROR` rows in the QA file fail Check 1; the pilot report lists them verbatim.
- Spot-light orientation (`pitch -90`) and right-leaf sign (`-OpenAngle`) are verified visually in Checks 4 and 6, not assumed.

## A.5 Blueprint reference (node-level)

### Interaction (BP_FirstPersonCharacter)
```
EnhancedInputAction IA_Interact (Started)
 → Start = FirstPersonCamera.GetWorldLocation
 → End   = Start + FirstPersonCamera.GetForwardVector * 250
 → LineTraceByChannel(Visibility, IgnoreSelf)
 → BreakHitResult.HitActor → Interact (Message, BPI_Interact)
```

### BP_Door
```
Components: DefaultSceneRoot → DoorPivot(Scene) → DoorMesh(StaticMesh); ProximityBox(BoxCollision)
Vars (Instance Editable): OpenAngle=90, AutoOpen=false, IsSliding=false, SlideDistance=100, IsLocked=false, PairedLeaf (BP_Door ref); IsOpen (private)
(Implemented as C++ base ASMHQDoor with UPROPERTY(EditAnywhere) — BP_Door is its Blueprint child; recommended for agent authoring)
Timeline TL_Door: float Alpha 0→1 over 1.2 s (auto tangents)
  Update: IsSliding ? DoorMesh.SetRelativeLocation((Lerp(0,SlideDistance,Alpha),0,0))
                    : DoorMesh.SetRelativeRotation((0,0,Lerp(0,OpenAngle,Alpha)))   // yaw
Event Interact: IsLocked ? PlaySound(Locked) : (IsOpen ? Reverse : Play); IsOpen = !IsOpen; if PairedLeaf valid → PairedLeaf.SetOpen(IsOpen) (no recursion)
ProximityBox.OnBeginOverlap: AutoOpen && OtherActor==GetPlayerPawn(0) → Play; IsOpen=true
ProximityBox.OnEndOverlap:   AutoOpen && OtherActor==GetPlayerPawn(0) → Reverse; IsOpen=false
```

### BP_RoomLights
```
Vars: RoomCode(Name), Lights(Array<Light>), EmissiveMeshes(Array<StaticMeshActor>), IsOn, StartOn
ConstructionScript: GetAllActorsWithTag(RoomCode) → cast to Light → Lights
SetLights(On): ForEach Lights → LightComponent.SetVisibility(On)
               ForEach EmissiveMeshes → StaticMeshComponent.SetScalarParameterValueOnMaterials("EmissiveStrength", On?1:0)
               IsOn = On
Toggle(): SetLights(!IsOn)
BeginPlay: SetLights(StartOn)
```

### BP_LightSwitch
```
Implements BPI_Interact; Var TargetRoom (BP_RoomLights, Instance Editable)
Event Interact → TargetRoom.Toggle → PlaySound(Click)
```

### BP_DayNight
```
Var SunSky (actor ref), DayTime=10.0, NightTime=20.0, IsNight
Timeline 3 s: SolarTime = Lerp(DayTime, NightTime, Alpha) → SunSky.SetEditorProperty/SolarTime + UpdateSun (call the SunSky BP's "UpdateSun" function)
On finish: ForEach BP_RoomLights → SetLights(IsNight)
```

### Teleport (WBP_Menu button)
```
OnClicked → GetAllActorsWithTag(TagName)[0] → loc/rot
 → PlayerCameraManager.StartCameraFade(0,1,0.3) → Delay 0.3
 → PlayerPawn.TeleportTo(loc, rot) → PlayerController.SetControlRotation(rot)
 → StartCameraFade(1,0,0.3)
```

### BP_InfoPoint
```
Components: Trigger(Sphere r=200), Marker(WidgetComponent, Space=Screen)
Vars: MaquetteCode (Name, row key into DT_Maquettes ← Maquettes.csv) → ProjectCode, Title, Description, Images, Video
OnBeginOverlap(player) → show prompt "Press E"
Event Interact → CreateWidget(WBP_InfoPanel) → set data → AddToViewport → SetInputMode GameAndUI, ShowMouseCursor
OnEndOverlap(player) → RemoveFromParent
```
`DT_Maquettes` row struct `S_Maquette { Project, Title, Description, Images[], Video }`, row name = `MQ-xx`, imported from `Maquettes.csv`.

### BP_CinemaScreen
```
Assets: MS_Showreel (FileMediaSource, file in Content/Movies), MP_Auditorium (MediaPlayer, Loop on), T_MP_Auditorium (MediaTexture)
M_Screen: TextureSample(T_MP_Auditorium) * Brightness → Emissive; BaseColor black; Roughness 0.3
Components: Screen(StaticMesh with M_Screen), MediaSound (Player=MP_Auditorium, attenuation ~3000 cm), HallTrigger(Box)
HallTrigger.OnBeginOverlap → MP_Auditorium.OpenSource(MS_Showreel) (PlayOnOpen=true); AudiLights.SetDim(0.1)
HallTrigger.OnEndOverlap   → MP_Auditorium.Pause; AudiLights.SetLights(true)
Event Interact → IsPlaying ? Pause : Play
```
Video: MP4, H.264, 1080p30, 10–15 Mbps. Must reside under `Content/Movies/` to be staged in packaged builds.

## A.6 Packaging & Pixel Streaming

### Windows package
- Project Settings → Packaging: Build Configuration **Shipping**, Use Pak File, Use Io Store, Include Prerequisites installer, add `Movies` to *Directories to Stage* if not auto-detected.
- Platforms → Windows → Package Project.
- Ship with `Saved/Config` defaults at Medium scalability; in-app quality menu via `GameUserSettings.SetOverallScalabilityLevel(n)` + `ApplySettings`.
- Smoke-test on a second machine (clean Windows, no UE installed).

### Pixel Streaming 2 (self-hosted on the RTX 5080)
1. Enable **Pixel Streaming 2** plugin; package (Shipping or Development).
2. Get **PixelStreamingInfrastructure** branch matching UE 5.8 (verify repo location/branch: <https://github.com/EpicGamesExt/PixelStreamingInfrastructure>).
3. Start the Signalling & Web Server (Node.js) from `SignallingWebServer/platform_scripts/cmd/` (script names vary by branch — verify).
4. Launch the app:
   ```
   SMHQ_Virtual.exe -PixelStreamingURL=ws://127.0.0.1:8888 -RenderOffscreen -ResX=1920 -ResY=1080 -Windowed
   ```
5. Browse to `http://<host>/`.
6. External access: TURN server (e.g., coturn) + port forwarding / reverse proxy with TLS, or a managed provider.
7. Capacity: plan **1 concurrent viewer per GPU instance** for full-quality interactive sessions (multi-viewer of the same session is possible; independent sessions need separate instances) *(verify for your quality target)*.

Docs: <https://dev.epicgames.com/documentation/en-us/unreal-engine/pixel-streaming-in-unreal-engine> · <https://dev.epicgames.com/documentation/en-us/unreal-engine/pixel-streaming-2-overview-in-unreal-engine>

## A.7 Performance budgets

| Target | Resolution | Scalability | FPS |
|---|---|---|---|
| Dev on current PC (Arc 140T iGPU, pilot only) | 1080p, SP 50% | Low (build) / Medium (Check 11) | ≥ 30 at Medium = Check 11 (locked) |
| Client .exe default | 1080p | Medium | ≥ 30 |
| New PC / showroom | 1440p–4K | Epic + HWRT + MegaLights | ≥ 60 (1440p) |
| Pixel Streaming | 1080p | High | 30–60 |

Low-RAM shader compile (current laptop): if the editor runs out of memory while compiling shaders, reduce parallel shader workers in `Config/DefaultEngine.ini` → `[DevOptions.Shaders]` (e.g. `NumUnusedShaderCompilingThreads=6`) *(verify key in 5.8)*.

Profiling: `stat unit`, `stat gpu`, `ProfileGPU` (Ctrl+Shift+,), Unreal Insights. Typical fixes: reduce shadow-casting lights, lower Lumen Final Gather, ensure Nanite on dense meshes, cap texture sizes at 2K for non-hero assets, stream floors.

## A.8 Official references (Epic)

- UE 5.8 Release Notes — <https://dev.epicgames.com/documentation/en-us/unreal-engine/unreal-engine-5-8-release-notes>
- Hardware & Software Specs — <https://dev.epicgames.com/documentation/en-us/unreal-engine/hardware-and-software-specifications-for-unreal-engine>
- Datasmith supported software — <https://dev.epicgames.com/documentation/en-us/unreal-engine/datasmith-supported-software-and-file-types>
- Using Datasmith with SketchUp Pro — <https://dev.epicgames.com/documentation/en-us/unreal-engine/using-datasmith-with-sketchup-pro>
- Exporting from SketchUp Pro — <https://dev.epicgames.com/documentation/en-us/unreal-engine/exporting-datasmith-content-from-sketchup-pro-to-unreal-engine>
- SketchUp Pro Exporter download — <https://www.unrealengine.com/en-US/plugins/sketchup-pro>
- Datasmith Import Options — <https://dev.epicgames.com/documentation/en-us/unreal-engine/datasmith-import-options>
- Datasmith Python API (DatasmithSceneElement) — <https://dev.epicgames.com/documentation/en-us/unreal-engine/python-api/class/DatasmithSceneElement>
- Customizing the Datasmith import process (UE 4.27, concepts still apply) — <https://docs.unrealengine.com/4.27/en-US/WorkingWithContent/Importing/Datasmith/Overview/CustomizingImportProcess>
- Python scripting in Unreal Editor — <https://dev.epicgames.com/documentation/en-us/unreal-engine/scripting-the-unreal-editor-using-python>
- Twinmotion pricing update (Epic) — <https://www.twinmotion.com/news/we-are-updating-twinmotion-pricing-in-late-april>
- Using Datasmith with Revit — <https://dev.epicgames.com/documentation/en-us/unreal-engine/using-datasmith-with-revit>
- Lumen — <https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-global-illumination-and-reflections-in-unreal-engine>
- Lumen Performance Guide — <https://dev.epicgames.com/documentation/en-us/unreal-engine/lumen-performance-guide-for-unreal-engine>
- Nanite — <https://dev.epicgames.com/documentation/en-us/unreal-engine/nanite-virtualized-geometry-in-unreal-engine>
- MegaLights — <https://dev.epicgames.com/documentation/en-us/unreal-engine/megalights-in-unreal-engine>
- Sun and Sky Actor — <https://dev.epicgames.com/documentation/en-us/unreal-engine/sun-and-sky-actor-in-unreal-engine>
- Physical Lighting Units — <https://dev.epicgames.com/documentation/en-us/unreal-engine/physical-lighting-units-in-unreal-engine>
- IES Light Profiles — <https://dev.epicgames.com/documentation/en-us/unreal-engine/using-ies-light-profiles-in-unreal-engine>
- Blueprints Quick Start — <https://dev.epicgames.com/documentation/en-us/unreal-engine/quick-start-guide-for-blueprints-visual-scripting-in-unreal-engine>
- Timelines — <https://dev.epicgames.com/documentation/en-us/unreal-engine/timelines-in-unreal-engine>
- Blueprint Interface — <https://dev.epicgames.com/documentation/en-us/unreal-engine/blueprint-interface-in-unreal-engine>
- Enhanced Input — <https://dev.epicgames.com/documentation/en-us/unreal-engine/enhanced-input-in-unreal-engine>
- UMG Quick Start — <https://dev.epicgames.com/documentation/en-us/unreal-engine/umg-ui-designer-quick-start-guide-in-unreal-engine>
- Data Driven Gameplay (Data Tables) — <https://dev.epicgames.com/documentation/en-us/unreal-engine/data-driven-gameplay-elements-in-unreal-engine>
- Play a Video File — <https://dev.epicgames.com/documentation/en-us/unreal-engine/play-a-video-file-in-unreal-engine>
- Media Framework — <https://dev.epicgames.com/documentation/en-us/unreal-engine/media-framework-in-unreal-engine>
- World Partition — <https://dev.epicgames.com/documentation/en-us/unreal-engine/world-partition-in-unreal-engine>
- Level Streaming — <https://dev.epicgames.com/documentation/en-us/unreal-engine/level-streaming-in-unreal-engine>
- Movie Render Queue — <https://dev.epicgames.com/documentation/en-us/unreal-engine/movie-render-pipeline-in-unreal-engine>
- Packaging Projects — <https://dev.epicgames.com/documentation/en-us/unreal-engine/packaging-unreal-engine-projects>
- Pixel Streaming — <https://dev.epicgames.com/documentation/en-us/unreal-engine/pixel-streaming-in-unreal-engine>
- Pixel Streaming 2 — <https://dev.epicgames.com/documentation/en-us/unreal-engine/pixel-streaming-2-overview-in-unreal-engine>
- Pricing update (Epic blog) — <https://www.unrealengine.com/en-US/blog/we-are-updating-unreal-engine-twinmotion-and-realitycapture-pricing-in-late-april>
- License — <https://www.unrealengine.com/en-US/license>
- Fab marketplace — <https://www.fab.com>

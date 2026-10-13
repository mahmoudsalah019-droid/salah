# خطة المكتب الافتراضي التفاعلي — مقر صلاح محجوب للعمارة والتصميم

**المحرك:** Unreal Engine 5.8 — **المصدر:** موديل SketchUp 2023 — **إعداد:** أكتوبر 2026 — **مراجعة 2:** 2026-10-13 (التسمية والـ Checks اتعتمدوا، الجهاز الحالي اتحدد، المدة اتحسبت تاني على فريق بالتوازي)

> **الحالة:** الخطة معتمدة للتجهيز بس. **التجربة في Unreal مش هتبدأ غير لما م. صلاح يقول**، بعد ما يراجع تعديلات الموديل في SketchUp.

> **ملحوظة عن المصادر:** كل اللينكات من التوثيق الرسمي لـ Epic (dev.epicgames.com وunrealengine.com)، وقت كتابة الخطة. أي رقم مكتوب جنبه **"يحتاج تحقق"** معناه إن لازم نراجعه على صفحة Epic أو عند المورّد قبل ما نعتمده، خصوصاً الأسعار. لو لينك اتنقل، دوّر بعنوان الصفحة في [Epic Developer Community](https://dev.epicgames.com/documentation/en-us/unreal-engine).

---

## 0. الخلاصة في صفحة واحدة

| البند | القرار |
|---|---|
| النقل من SketchUp | **Datasmith** (مش FBX). بيحافظ على الأسماء والخامات ونقط اللف ونسخ الكومبوننتس. |
| التفاعل | **Blueprint** من غير كود: باب، نور، تمشية، تنقّل سريع، نقط معلومات، فيديو. |
| الإضاءة | **Lumen** للإضاءة المرتدة والانعكاسات، و**Nanite** للتفاصيل التقيلة، و**SunSky** على موقع جدة الحقيقي. |
| الجهاز الحالي | Intel Arc 140T **مدمج** (بياخد لحد 16 جيجا من الرامات): نطاق التجربة بس، بجودة Low/Medium (**Lumen Lite**) ودقة مخفّضة. للبلوبرنت والتفاعل، مش للشكل النهائي. |
| الجهاز الجديد RTX 5080 | جودة Epic، و**Hardware Ray Tracing**، و**MegaLights** للإضاءة الليلية الكتير، وهو كمان اللي هيعمل التغليف (Packaging) ويشغّل البث. |
| التسليم | (1) برنامج ويندوز `.exe` للعميل اللي عنده جهاز كويس. (2) لينك من المتصفح بـ **Pixel Streaming** للباقي. (3) فيديو وصور 360 كاحتياطي للموبايل. |
| نبدأ بإيه | تجربة أولى **أسبوع واحد** (5 أيام شغل بفريق بالتوازي): البهو والاستقبال، باب واحد يتفتح، ومجموعة نور تولّع وتطفي، ونقطة معلومات واحدة. **بتبدأ لما م. صلاح يقول بس.** |
| المدة | حوالي **6 أسابيع** للدور الأرضي جاهز للعميل (+ أسبوع احتياطي = 7)، بفريق 3 أشخاص وسكربتات. الأدوار اللي فوق 2–3 أيام للدور المكرر لما تصميمها يخلص. |

---

## 1. نقل الموديل من SketchUp لـ Unreal

### 1.1 Datasmith ولا FBX؟ — **Datasmith**

| | Datasmith | FBX |
|---|---|---|
| أسماء العناصر (الأبواب والإضاءة) | بتتنقل زي ما هي | بتتنقل بس ساعات بتتلخبط مع الجروبات |
| نقطة اللف (المفصّلة) | بتتنقل من محاور الـ Component | محتاجة ظبط يدوي غالباً |
| الخامات | بتتنقل بأساميها وصورها | بتتنقل ناقصة وساعات بأسماء غريبة |
| تكرار نفس العنصر (100 كرسي) | بيتنقل كنسخ من عنصر واحد → أخف بكتير | ساعات بيتحول لـ 100 عنصر منفصل |
| التحديث | **DirectLink**: تعدّل في SketchUp وتضغط Sync | لازم تعيد التصدير كله |
| الكاميرات (Scenes) | بتتنقل | لأ |

**القرار:** Datasmith للمبنى كله. FBX نستخدمه بس لعنصر لوحده جاي من برنامج تاني (زي كرسي جاهز أو ماكيت متعمل في 3ds Max).

المصادر:
- [Datasmith Supported Software and File Types](https://dev.epicgames.com/documentation/en-us/unreal-engine/datasmith-supported-software-and-file-types)
- [Using Datasmith with SketchUp Pro](https://dev.epicgames.com/documentation/en-us/unreal-engine/using-datasmith-with-sketchup-pro)
- [Exporting Datasmith Content from SketchUp Pro](https://dev.epicgames.com/documentation/en-us/unreal-engine/exporting-datasmith-content-from-sketchup-pro-to-unreal-engine)
- [SketchUp Pro Exporter (تحميل البلجن)](https://www.unrealengine.com/en-US/plugins/sketchup-pro) — صفحة Epic بتقول إنه بيدعم UE 5.8 وSketchUp Pro من 2019 لـ 2026. SketchUp 2023 اللي عندنا جوه المدى ده. **يحتاج تحقق** يوم 1 إن الـ Sync بـ DirectLink شغال عليها (فيه شكاوى قديمة في منتدى SketchUp من مشاكل Sync مع بعض النسخ)، ولو فيه مشكلة نصدّر ملف `.udatasmith` يدوي بدل DirectLink.
- [Datasmith DirectLink](https://dev.epicgames.com/documentation/en-us/unreal-engine/datasmith-directlink-in-unreal-engine)

### 1.2 تجهيز الموديل في SketchUp قبل التصدير (مراجعة نهائية)

القواعد بتاعتك ممتازة. دي الحاجات اللي هنتأكد منها زيادة:

1. **الباب لازم يكون Component مش Group.** ونقطة أصل الكومبوننت (المحاور الحمرا والخضرا والزرقا) تكون على محور المفصّلة بالظبط، والمحور الأزرق رأسي لفوق. ده اللي Unreal بيعتبره نقطة اللف.
   - طريقة الظبط: كليك يمين على الباب ← **Change Axes** ← حط الأصل على المفصّلة السفلية.
   - الحلق (الإطار) يفضل **برّه** كومبوننت الضلفة، علشان الحلق ثابت والضلفة بس هي اللي تلف.
   - الباب بضلفتين = كومبوننتين، كل واحد ليه مفصّلته.
   - الباب السحّاب: نفس الفكرة، والأصل في أي ركن، وهيتحرك على خط بدل ما يلف.
   - **اسم الباب `D-xx` يتكتب في اسم الـ Instance** (Entity Info ← Instance)، مش بس في اسم الـ Definition. لو 10 أبواب نفس الموديل، هيبقوا Definition واحدة و10 Instances كل واحد باسمه. هنتأكد في التجربة إن Datasmith بينقل اسم الـ Instance (Check 1).
2. **وحدات الإضاءة:** نفس الكلام: Component، واسمها الثابت في اسم الـ Instance. SketchUp مفيهوش لمبات حقيقية، فاللي هيتنقل هو **شكل الوحدة** (الجسم). في Unreal هنحط جوه كل وحدة لمبة حقيقية أوتوماتيك بالاسم (شوف الملحق التقني A.4). علشان كده الاسم الثابت مهم جداً.
3. **الوشوش المقلوبة:** في SketchUp، أي وش أزرق-رمادي (Back Face) باين للناس هيطلع غلط في Unreal. استخدم **View ← Face Style ← Monochrome** وصلّح أي وش مقلوب (**Reverse Faces**).
4. **الخامة على الوش الأمامي بس**، مش على الجروب من برّه ولا على الوش الخلفي.
5. **امسح الحاجات المستخبية والزيادة:** **Window ← Model Info ← Statistics ← Purge Unused**، وامسح أي Tags مقفولة مش محتاجينها.
6. **الزجاج:** خامة باسم واضح فيها كلمة Glass (زي `Glass_Clear_Low-E`) ووش واحد بس، مش صندوق.
7. **قسّم الموديل بالـ Tags:** Tag لكل دور (`GF`, `L01`…`L06`, `ROOF`) وTag لكل نوع (`ARCH`, `FURN`, `DOOR`, `LIGHT`, `GLASS`). ده هيسهّل علينا نفتح دور واحد بس على الجهاز الضعيف.
8. **ملفات منفصلة لكل دور** (موصى بيه): ملف `HQ_GF.skp` للأرضي، وملف لكل دور فوق، على نفس نقطة الأصل (0,0,0). كده نقدر ننقل ونحدّث كل دور لوحده.
9. **المتر:** تمام، Datasmith بيحوّل للسنتيمتر اللي Unreal بيشتغل بيه لوحده. هنتأكد في التجربة الأولى إن ارتفاع الباب 2.10 م بيطلع 210 سم.

### 1.3 نظام التسمية (معتمد 2026-10-13)

**القاعدة:** Unreal هو اللي يتكيّف مع الموديل، مش العكس. مفيش إعادة تسمية في SketchUp غير اللي إنت قررته.

| العنصر | الاسم في SketchUp | ملاحظة |
|---|---|---|
| ضلفة باب | `D-xx` (مثلاً `D-12`) | Component، محاوره على المفصّلة، والاسم في الـ Instance |
| فراغ (أوضة/منطقة) | `E-xx` (مثلاً `E-03`) | |
| الخامات | زي ما هي بالظبط | |
| وحدة إضاءة | الاسم الثابت الحالي | Component، والاسم في الـ Instance |

**كل حاجة مش في الاسم بتتكتب في جداول** (Excel يتحفظ CSV) — نفس فكرة جدول الأبواب المعماري، والسكربت هو اللي بيقراها:

| الجدول | الأعمدة | مثال |
|---|---|---|
| `Doors.csv` | Code, Space, Type (Swing / Sliding), OpenAngle, AutoOpen, Locked | `D-12, E-03, Swing, 90, No, No` |
| `Lights.csv` | Name, Space, Kind (DOWN / SPOT / PEND / WALL / LINE / COVE), Lumens, Kelvin, IES | `<اسم الوحدة>, E-01, DOWN, 900, 3000, <ملف IES>` |
| `Spaces.csv` | Code, NameAR, NameEN, Floor, Teleport (Yes/No) | `E-01, الاستقبال, Reception, GF, Yes` |
| `Projects.csv` | Code, Title, Description, Images, Video | `D-07, …` (لنقط المعلومات) |

ميزة الجداول: أي تعديل (زاوية فتح باب، شدة نور، باب يتقفل) بيتعمل في الإكسل ويتعاد تشغيل السكربت في دقايق، من غير ما حد يفتح Unreal.

> **⚠ تعارض لازم ناخد فيه قرار:** أكواد مشاريع البورتفوليو كمان `D-01` لحد `D-24`، وهي اللي هتظهر على نقط المعلومات في المعرض. علشان مايحصلش لخبطة:
> 1. السكربت مش هيعتبر أي حاجة اسمها بيبدأ بـ `D-` باب؛ **الأبواب هي اللي في `Doors.csv` بس**.
> 2. جوه Unreal، بيانات المشاريع مفتاحها `PRJ_D-07`، والزائر بيشوف `D-07` عادي.
> 3. **الماكيتات نفسها في SketchUp ماتتسماش `D-xx`.** اقتراح: `M-xx` أو اسم المشروع. (محتاج قرارك.)
>
> **محتاج تأكيد كمان:** الباب بضلفتين بيتسمى إزاي؟ اقتراح: `D-12-L` و`D-12-R`، وكل ضلفة سطر في `Doors.csv` (واحدة 90 والتانية −90). وهل أرقام `D-xx` و`E-xx` مش بتتكرر في المبنى كله، ولا بتبدأ من الأول في كل دور؟ لو بتتكرر، هنضيف عمود Floor للجداول.

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
| Asset Conflict Policy | Update | علشان التحديث بعدين يكمّل على نفس العناصر |
| Actor Conflict Policy | Update | نفس السبب |

المرجع: [Datasmith Import Options](https://dev.epicgames.com/documentation/en-us/unreal-engine/datasmith-import-options)

9. **شغّل Nanite على الحاجات التقيلة:** في الـ Content Browser افتح `/Game/SMHQ/GF/Geometries`، اختار الأشكال المعتمة (كراسي القاعة، الماكيتات، الواجهات، الدرابزين، الحديقة) ← كليك يمين ← **Nanite ← Enable**. **ماتشغّلوش على الزجاج** (الحاجات الشفافة مش بتشتغل مع Nanite). [Nanite Virtualized Geometry](https://dev.epicgames.com/documentation/en-us/unreal-engine/nanite-virtualized-geometry-in-unreal-engine)
10. **تحقق سريع:**
    - عدّي على 3 أبواب واتأكد إن الـ Pivot (الأسهم لما تختار الباب) على المفصّلة، وإن اسمها في الـ Outliner هو `D-xx` نفسه.
    - قيس باب: في نافذة جانبية (Front أو Left) اسحب بزرار الماوس الأوسط من تحت لفوق الباب، هيظهرلك المقاس. لازم 210 لو الباب 2.10 م.
    - دوّر على أي حتة سودا أو شفافة غلط = وش مقلوب ← ارجع SketchUp صلّحه.

**التحديث بعد كده (DirectLink):**
- في SketchUp: **Datasmith Toolbar ← Sync** (أو Auto Sync).
- في Unreal: اختار DatasmithSceneActor ← في الـ Details ← **Connect to DirectLink source** ← اختار ملف SketchUp ← **Update**.
- **تحذير:** أي تعديل عملناه **جوه** عناصر Datasmith في Unreal ممكن يترجع مع التحديث. علشان كده **كل التفاعل بيتعمل في Blueprints منفصلة بنحطها جنب الموديل، مش بنعدّل في الموديل نفسه** (اتشرح تحت).

### 1.6 فين Twinmotion في الحكاية؟

Twinmotion 2026.1 ممتاز للقطات سريعة وفيديو للعميل في أيام، بس التفاعل اللي إنت طالبه (أبواب، مفاتيح نور، نقط معلومات، فيديو على شاشة، تسليم ويندوز ومتصفح) مكانه Unreal. الاقتراح:
- **Twinmotion:** صور وفيديو عرض سريع بالتوازي، من نفس ملف SketchUp.
- **Unreal:** المكتب الافتراضي التفاعلي.
- ماننقلش من Twinmotion لـ Unreal؛ ننقل من SketchUp لـ Unreal على طول، علشان يبقى عندنا مصدر واحد للحقيقة.

---

## 2. التفاعل بالبلوبرنت — خطوة بخطوة

### 2.0 قبل ما نبدأ: كلمتين عن البلوبرنت

- **Blueprint** = "عنصر ذكي": شكل + تصرّف. بتبنيه بتوصيل مربعات (Nodes) بخيوط، من غير كتابة كود.
- **الحدث (Event)** = "لما يحصل كذا" (مربع أحمر). **الفعل (Function)** = "اعمل كذا" (مربع أزرق).
- **المتغيّر (Variable)** = خانة بنحط فيها معلومة (مثلاً: الباب مفتوح؟ نعم/لا).
- لو علّمت على المتغيّر **Instance Editable** (العين المفتوحة)، تقدر تغيّر قيمته لكل نسخة من الـ Level من غير ما تفتح البلوبرنت. ده اللي هيخلّي حد مش مبرمج يحط 200 باب ويظبطهم من الـ Details بس.
- كل البلوبرنتس هنحطها في `/Game/SMHQ/Blueprints/`.

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
2. في Details ← `DoorMesh` ← **Static Mesh** ← اختار شكل الضلفة اللي جه من Datasmith (`D-12`).
3. حط الـ BP_Door بالظبط في مكان الضلفة الأصلية. **أسهل طريقة:** اختار الضلفة الأصلية ← انسخ الـ Location والـ Rotation من الـ Details ← الصقهم في الـ BP_Door.
4. اخفي الضلفة الأصلية اللي من Datasmith (**Actor Hidden In Game ✔** و**Visible ✖**). ماتمسحهاش، علشان DirectLink مايرجعهاش.
5. علشان 200 باب: السكربت في الملحق A.4 بيعمل الخطوات دي أوتوماتيك لكل باب مكتوب في `Doors.csv`، وبياخد منه النوع والزاوية والفتح الأوتوماتيك والقفل.

**و. صوت:** نحط صوت فتح وقفل (Sound Cue) على **Play** و**Reverse**. يفرق في الإحساس جداً.

### 2.3 تشغيل وتطفية النور في كل أوضة

**الفكرة:** كل أوضة فيها **مدير نور** واحد `BP_RoomLights`، وفيها مفتاح أو أكتر على الحيطة `BP_LightSwitch` بيكلّم المدير.

**أ. `BP_RoomLights`:**
1. **Blueprint Class ← Actor** ← `BP_RoomLights`. ملوش شكل؛ بيتحط في نص الأوضة.
2. متغيّرات Instance Editable:
   | المتغيّر | النوع | معناه |
   |---|---|---|
   | `RoomCode` | Name | كود الفراغ، مثلاً `E-01` |
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
6. **ملء القائمة أوتوماتيك:** بدل ما تختار اللمبات واحدة واحدة، في **Construction Script**: **Get All Actors with Tag** (التاج = `RoomCode`) ← حطهم في `Lights`. والسكربت (ملحق A.4) بيدي كل لمبة تاج فراغها من عمود Space في `Lights.csv` (مثلاً `E-01`).

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

**`BP_InfoPoint` بلوبرنت واحد لكل المعروضات:**

1. **Actor** فيه:
   - `Trigger` (**Sphere Collision**، نص قطر حوالي 200 سم).
   - `Marker` (**Widget Component** صغير "i" عايم فوق الماكيت ويلف ناحية الشخص: **Space = Screen**).
2. Interface: `BPI_Interact`.
3. متغيّرات Instance Editable:
   | المتغيّر | النوع | مثال |
   |---|---|---|
   | `ProjectCode` | Text | D-07 |
   | `Title` | Text | فيلا خاصة — أبحر |
   | `Description` | Text | سطرين: النوع، المساحة، السنة |
   | `Images` | Array of Texture 2D | صور المشروع |
   | `Video` | File Media Source | (اختياري) فيديو المشروع |
4. **لما تقرب** (Begin Overlap للاعب): يظهر تحت الشاشة "**اضغط E لعرض المشروع**".
5. **Event Interact** ← يعمل **Create Widget** من `WBP_InfoPanel` ويبعتله البيانات ← **Add to Viewport**:
   - `WBP_InfoPanel` فيه: عنوان، وصف، صورة كبيرة بأسهم يمين/شمال، وزرار "تشغيل الفيديو" لو فيه فيديو، وزرار إغلاق (أو **Esc**).
   - الفيديو جوه الـ Panel: **Image** خامته من Media Texture (نفس طريقة 2.6).
6. **لما تبعد** (End Overlap): تتقفل لوحدها.
7. **مصدر البيانات (اختياري ومفيد جداً):** بدل ما نكتب بيانات كل مشروع في الـ Details، نعمل **Data Table** من ملف CSV/Excel فيه D-01 لحد D-24 (الكود، الاسم، الوصف، الصور). ونقطة المعلومات تسحب بياناتها بالكود. ساعتها تحديث المعرض = تعديل الإكسل وإعادة استيراده. [Data Driven Gameplay Elements](https://dev.epicgames.com/documentation/en-us/unreal-engine/data-driven-gameplay-elements-in-unreal-engine)

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
4. **الأتريوم والسكاي لايت:** ده أهم مكان بصرياً. Lumen هيعمل النور المرتد من الحديقة على الحيطان لوحده. اتأكد إن زجاج السقف خامته شفافة رفيعة (Thin Translucent) مش صندوق مقفول، وإلا الشمس مش هتدخل.
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
| `M_Master_Glass` | زجاج الواجهة والفواصل والسكاي لايت | لون خفيف، Roughness ≈ 0.05، Thin Translucent |
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

**دوره في المشروع:** نبني ونجرّب عليه **التفاعل والبلوبرنتس** في نطاق التجربة (البهو والاستقبال). **الشكل النهائي** للإضاءة والخامات بيتقيّم على الجهاز الجديد.

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
- **لسه محتاج الإجابة:** الرد وصل فيه `[تحت / فوق]` من غير اختيار. لو **تحت** المليون: Unreal ببلاش، وTwinmotion حسب ترخيصه الحالي. لو **فوق**: اشتراك Unreal لكل مقعد (لكل واحد في الفريق بيشتغل على Unreal) + Twinmotion منفصل.
- المراجعة النهائية: [Unreal Engine License / EULA](https://www.unrealengine.com/en-US/license).

### 5.5 ملخص التكلفة (تقديري — بالدولار)

| البند | مرة واحدة | سنوي/شهري | ملاحظة |
|---|---|---|---|
| ترخيص Unreal | — | 0 أو حوالي 1,850/مقعد/سنة | حسب إيراد المكتب — **يحتاج تحقق** |
| ترخيص Twinmotion | — | 0 أو حوالي 445/مقعد/سنة | منفصل من مايو 2026 — **يحتاج تحقق** |
| أصول Fab (شجر، خامات، أثاث تكميلي، أصوات) | 0–1,000 | — | كتير منها مجاني؛ نختار حسب الترخيص |
| الجهاز الجديد | (اشتريته) | — | — |
| Pixel Streaming من المكتب | — | ~0 + نت | محتاج Upload كويس |
| Pixel Streaming خدمة جاهزة | — | عشرات–مئات/شهر | **يحتاج تحقق** بعرض سعر |
| سيرفر سحابي | — | ~0.5–1.5/ساعة تشغيل | **يحتاج تحقق** |
| استضافة الـ .exe للتحميل (Google Drive / OneDrive) | — | موجود غالباً | — |

---

## 6. التجربة الأولى: البهو والاستقبال (أسبوع — بتبدأ بإذن م. صلاح بس)

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

**الحاجات اللي الـ Checks دي مابتقيسهاش** (هنعلّق عليها في التقرير): الإحساس العام بالواقعية، توازن ألوان الخامات، راحة سرعة المشي. دي تقييمك إنت بالعين.

### 6.3 قبل التجربة: تعديلات الموديل (عند فريق SketchUp)

1. الأبواب ووحدات الإضاءة Components، ومحاور الأبواب على المفصّلة.
2. الأسماء في الـ Instances: `D-xx`، `E-xx`، وأسماء الإضاءة الثابتة.
3. الجداول الأربعة (`Doors.csv`، `Lights.csv`، `Spaces.csv`، `Projects.csv`) لنطاق التجربة على الأقل.
4. قرار تسمية الماكيتات والأبواب بضلفتين (1.3).
5. فصل نطاق التجربة في ملف `HQ_GF_Pilot.skp`.

**بعدها م. صلاح يراجع ويقول "ابدأ"، وساعتها بس نفتح Unreal.**

### 6.4 التجربة يوم بيوم (5 أيام شغل، 3 أشخاص بالتوازي)

| اليوم | (A) فني Unreal / بلوبرنت | (B) إضاءة وخامات | (C) بيانات وواجهة |
|---|---|---|---|
| 1 | تنزيل بلجن Datasmith، المشروع، البلجنز، الاستيراد، Nanite، Collision، ارتفاع العين. **Checks 1–3، 8** | SunSky على جدة، Post Process، Exposure | مراجعة الجداول وتحويلها CSV، صور ونصوص المشروع للنقطة، تصميم القايمة |
| 2 | `BPI_Interact` + `BP_Door` + تشغيل سكربت الأبواب. **Checks 4–5** | تشغيل سكربت الإضاءة، IES، درجات اللون، نهار وليل مبدئي | `WBP_Menu` (نقطتين تنقّل + نهار/ليل) |
| 3 | `BP_RoomLights` + `BP_LightSwitch` + `BP_DayNight`. **Checks 6–7** | 4–6 خامات PBR أساسية + تفاصيل حياة | `BP_InfoPoint` + `WBP_InfoPanel`. **Checks 9–10** |
| 4 | قياس الأداء (**Check 11**) وتخفيف لو محتاج، اختبار DirectLink (**Check 12**) | تظبيط نهائي للإضاءة على قد ما الجهاز يسمح | تغليف `.exe` وتجربته على جهاز تاني (**Check 13**) |
| 5 | تقرير التقييم: جدول الـ 13 Check (ناجح / فاشل / يحتاج تحقق) + السبب | فيديو قصير 1–2 دقيقة | مراجعة مع م. صلاح وقرار نكمل ولا نعدّل الطريقة |

لو الفريق شخصين، التجربة تبقى حوالي 7–8 أيام شغل. لو شخص واحد، حوالي أسبوعين.

## 7. الجدول الزمني والمخاطر

### 7.1 الجدول الزمني (محسوب تاني: فريق 3 أشخاص بالتوازي + سكربتات)

**الافتراضات:** (A) فني Unreal/بلوبرنت، (B) إضاءة وخامات، (C) بيانات وواجهة ومحتوى، وم. صلاح بيراجع. السكربتات بتبني الموديل وتعيد التصدير في دقايق، وسكربت Unreal (A.4) بيركّب الأبواب والإضاءة من الجداول في دقايق. الأسابيع = أسابيع شغل (أحد–خميس). **العد بيبدأ من يوم ما تقول "ابدأ"**، مش من النهارده.

| المرحلة | التوقيت | (A) | (B) | (C) |
|---|---|---|---|---|
| تجهيز الموديل والجداول | **قبل العد** (عندك) | — | — | — |
| 1. التجربة الأولى | أسبوع 1 | (6.4) | (6.4) | (6.4) |
| **نقطة قرار** | آخر أسبوع 1 | | | |
| 2. الدور الأرضي كامل | أسبوع 2–4 | كل الأبواب بالسكربت + ظبط، الشاشة والفيديو، الأصوات، الأداء | كل الفراغات نهار وليل، الأتريوم، قاعة العرض، المعرض، الشجر | كل الخامات PBR، D-01…D-24 في `Projects.csv`، القايمة الكاملة والخريطة |
| الجهاز الجديد | **لازم يوصل قبل أسبوع 3** | | الإضاءة النهائية عليه | |
| 3. التسليم | أسبوع 5 | تغليف `.exe` + Pixel Streaming من المكتب | فيديو جولة وصور 360 | دليل استخدام صفحة واحدة للعميل |
| 4. اختبار وتصحيح | أسبوع 6 | تصليح | تصليح | تجربة مع 3–5 أشخاص من برّه |
| احتياطي | أسبوع 7 | | | |
| 5. الأدوار اللي فوق + الملحق | لما تصميمها يخلص | 2–3 أيام للدور المكرر، وأسبوع تقريباً للدور المختلف (تقدير — **يحتاج تحقق** بعد الدور الأرضي) | | |

**الإجمالي:** حوالي **6 أسابيع** للدور الأرضي جاهز للعميل، **+ أسبوع احتياطي = 7**. (بشخصين: حوالي 8–9 أسابيع. بشخص واحد: حوالي 11–12.)

**إيه اللي السكربتات بتختصره وإيه اللي لأ:**

| بيتختصر | مابيتختصرش |
|---|---|
| بناء الموديل وإعادة التصدير | جولات المراجعة معاك (كل جولة 1–2 يوم) |
| تركيب 200 باب ومئات اللمبات | تظبيط الإضاءة والخامات بالعين — ده شغل فني |
| تعديل زوايا وشدّات من الإكسل | أول تجميع للـ Shaders (ساعات، مرة واحدة على كل جهاز) |
| نقل البيانات (D-01…D-24) | وصول الجهاز الجديد |
| | ظبط Pixel Streaming على شبكة المكتب مع IT |
| | تجربة ناس من برّه وتصليح ملاحظاتهم |

**المسار الحرج:** موافقتك على الموديل ← التجربة ← **وصول الجهاز الجديد قبل أسبوع 3** ← الإضاءة النهائية ← التسليم. لو الجهاز اتأخر، كل أسبوع تأخير بيأخر التسليم أسبوع تقريباً، حتى لو الفريق أكبر.

### 7.2 المخاطر

| # | الخطر | الاحتمال | التأثير | الحل |
|---|---|---|---|---|
| 1 | الكارت المدمج (Arc 140T) مايوصلش لـ 30 fps في التجربة (Check 11)، أو الرامات تخلص | عالي | متوسط | نطاق التجربة بس، Low/Medium، والبرامج التانية مقفولة. **Check 11 مش بيتسهّل**: لو فشل يتكتب فاشل وإنت تقرر |
| 2 | تأخر وصول الجهاز الجديد بعد أسبوع 3 | متوسط | **عالي جداً** (بقى على المسار الحرج بعد ضغط الجدول) | نطلبه دلوقتي؛ التفاعل بيتعمل على الحالي؛ كل أسبوع تأخير = أسبوع على التسليم |
| 3 | الأبواب نقطة لفها غلط (Group بدل Component، أو محاور غلط) | متوسط | متوسط | مراجعة في SketchUp قبل التصدير + Check 4 في التجربة |
| 4 | DirectLink بيرجّع تعديلات اتعملت في Unreal | متوسط | متوسط | **ممنوع** نعدّل في عناصر Datasmith؛ كل حاجة في Blueprints منفصلة |
| 5 | بلجن Datasmith لـ SketchUp مش متوافق مع نسخة SketchUp عندنا أو فيه مشاكل Sync | منخفض–متوسط | عالي | نتأكد من النسخة يوم 1؛ البديل: تصدير `.udatasmith` يدوي بدل DirectLink، وأسوأ حالة FBX |
| 6 | تسريب نور (حيطان وش واحد) أو Noise في Lumen | عالي | متوسط | حيطان بسُمك في SketchUp؛ Final Gather أعلى على الجديد |
| 7 | الفيديو مش بيشتغل عند العميل (شاشة سودا) | متوسط | متوسط | الفيديو في `Content/Movies`، H.264، تجربة على جهاز تاني قبل التسليم |
| 8 | جهاز العميل ضعيف للـ .exe | عالي | متوسط | قايمة جودة + Lumen Lite افتراضي + بديل Pixel Streaming |
| 9 | Pixel Streaming من المكتب ببطء أو مايفتحش برّه الشبكة | متوسط | متوسط | TURN Server؛ أو خدمة جاهزة |
| 10 | تكلفة الترخيص (لو الإيراد فوق المليون) والتغيير في Twinmotion من مايو 2026 | متوسط | منخفض–متوسط | نتأكد من صفحة Epic قبل الشراء |
| 11 | حجم الملفات وضياع شغل | متوسط | عالي | نسخ احتياطي يومي + نظام إصدارات (Git LFS / Perforce) |
| 12 | الأدوار اللي فوق لسه متصممتش، والعميل يتوقع المبنى كله | متوسط | متوسط | نوضح من الأول إن النسخة الأولى = الأرضي + الواجهة الخارجية كاملة؛ والأدوار تتضاف تباعاً |
| 13 | ترخيص الأصول (شجر، أثاث، أصوات) مش مسموح بيه في التوزيع | منخفض | عالي | جدول بكل أصل ومصدره وترخيصه |
| 14 | 5.8 آخر إصدار رئيسي في UE5 حسب Epic، وUE6 جاي | منخفض | منخفض | نثبت المشروع على 5.8 لحد التسليم؛ ماننقلش لإصدار جديد في نص الشغل |
| 15 | لخبطة بين أكواد الأبواب `D-xx` وأكواد المشاريع `D-01…D-24` | متوسط | متوسط | الأبواب من `Doors.csv` بس؛ الماكيتات مايتسموش `D-xx` (1.3) |
| 16 | Datasmith بينقل اسم الـ Definition بدل اسم الـ Instance، أو بيضيف لاحقة (`D-12_2`) | متوسط | متوسط | السكربت بيطلع تقرير بالأبواب اللي مالقاهاش؛ ده جزء من Check 1 |
| 17 | الجدول المضغوط بيعتمد على جولات مراجعة سريعة | متوسط | متوسط | ميعاد مراجعة ثابت مرتين في الأسبوع (مثلاً الاتنين والأربع) |

---

## 8. اللي اتقرر واللي لسه مستني

**اتقرر (2026-10-13):**
- التسمية: `D-xx` للأبواب، `E-xx` للفراغات، الخامات زي ما هي.
- الـ 13 Check بتوع التجربة: معتمدين ومقفولين.
- الجهاز الحالي: Core Ultra 7 255H + Arc 140T مدمج (لحد 16 جيجا مشتركة).
- SketchUp 2023.
- **التجربة في Unreal مابتبدأش غير لما م. صلاح يقول.**

**لسه محتاج منك:**
1. الإيراد السنوي: **تحت** ولا **فوق** مليون دولار؟ (الرد جه فيه الاختيارين.)
2. حجم الفريق اللي هيشتغل على Unreal فعلاً (الجدول محسوب على 3).
3. تسمية الماكيتات (مش `D-xx`) والأبواب بضلفتين (`D-12-L` / `D-12-R`؟).
4. أرقام `D-xx` و`E-xx` مش بتتكرر في المبنى كله، ولا كل دور ليه ترقيمه؟
5. ميعاد وصول الجهاز الجديد.
6. فيديو الـ Showreel، وصور ونصوص المشاريع D-01 لحد D-24.

## A.1 Project setup

| Item | Value |
|---|---|
| Template | Games → First Person, Blueprint, Starter Content off |
| Project name | `SMHQ_Virtual` |
| Required plugins | Datasmith Importer, Datasmith Content, Sun Position Calculator, Python Editor Script Plugin, Editor Scripting Utilities, Pixel Streaming 2 (delivery), Media Player (built in) |
| Folder layout | `/Game/SMHQ/{GF,L01..L06,ROOF}/`, `/Game/SMHQ/Blueprints/`, `/Game/SMHQ/Materials/Masters/`, `/Game/SMHQ/UI/`, `/Game/SMHQ/Data/`, `/Content/Movies/` |
| Maps | `L_HQ_Persistent` (lighting, SunSky, PPV, player start) + streamed sublevels `L_HQ_GF`, `L_HQ_L01` … or World Partition with Data Layers per floor |
| Source control | Git LFS or Perforce; commit `.uasset/.umap`, ignore `Saved/`, `Intermediate/`, `DerivedDataCache/` |

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
- Glass = single-face geometry, material name contains `Glass`.
- Walls/slabs with real thickness (≥ 10 cm) to avoid Lumen light leaks.
- Tags: floor (`GF`, `L01`…`L06`, `ROOF`) + category (`ARCH`, `FURN`, `DOOR`, `LIGHT`, `GLASS`).
- Purge unused; meters; shared origin (0,0,0) across per-floor files.
- Scenes for key viewpoints → become CineCameraActors → reusable as teleport anchors.

Naming (approved 2026-10-13): door leaves `D-xx` (double: `D-xx-L` / `D-xx-R`, pending confirmation), spaces `E-xx`, materials unchanged, light fixtures keep existing fixed names. Names must be set on the **component instance** (Entity Info → Instance), not only the definition.
Non-name attributes live in CSV schedules under `<Project>/SMHQ_Data/`:

| File | Columns |
|---|---|
| `Doors.csv` | `Code,Space,Type,OpenAngle,AutoOpen,Locked` (Type = Swing \| Sliding; booleans Yes/No) |
| `Lights.csv` | `Name,Space,Kind,Lumens,Kelvin,IES` (Kind = DOWN \| SPOT \| PEND \| WALL \| LINE \| COVE; IES = asset path or empty) |
| `Spaces.csv` | `Code,NameAR,NameEN,Floor,Teleport` |
| `Projects.csv` | `Code,Title,Description,Images,Video` → imported to `DT_Projects`, row key `PRJ_<Code>` |

Portfolio codes `D-01…D-24` collide with door codes: doors are identified **only** via `Doors.csv`, never by prefix; exhibit models must not be named `D-xx`.

## A.3 Datasmith import options

| Option | Value |
|---|---|
| Geometry / Materials & Textures / Cameras | On |
| Lights | On (SketchUp has none; harmless) |
| Generate Lightmap UVs | Off (Lumen) |
| Asset Conflict Policy | Update |
| Actor Conflict Policy | Update |
| Post-import | Enable Nanite on opaque high-poly meshes (Content Browser → right-click → Nanite → Enable). Never on translucent. |
| DirectLink | DatasmithSceneActor → Details → Connect to DirectLink source → Update |

Rule: **never modify Datasmith-owned actors** beyond visibility; all interactivity lives in separate Blueprint actors referencing the imported meshes.

## A.4 Editor Python: build doors and lights from the CSV schedules

Enable **Python Editor Script Plugin** + **Editor Scripting Utilities**. Run from *Tools → Execute Python Script*. Save the level first; the whole run is one undo transaction. Validate on the pilot level before any wider use.

```python
# smhq_autobuild.py — Unreal Engine 5.8 editor-only script
import csv, os, re
import unreal

DATA_DIR = os.path.join(unreal.Paths.project_dir(), "SMHQ_Data")
DOOR_BP  = unreal.EditorAssetLibrary.load_blueprint_class("/Game/SMHQ/Blueprints/BP_Door")
actor_sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

LIGHT_CLASS = {
    "DOWN": unreal.SpotLight, "SPOT": unreal.SpotLight,
    "PEND": unreal.PointLight, "WALL": unreal.PointLight,
    "LINE": unreal.RectLight,  "COVE": unreal.RectLight,
}

def load_csv(name, key):
    with open(os.path.join(DATA_DIR, name), newline="", encoding="utf-8-sig") as f:
        return {row[key].strip(): row for row in csv.DictReader(f)}

def yes(v):
    return str(v).strip().lower() in ("yes", "y", "true", "1")

def base_label(label):
    # Datasmith may suffix duplicates ("D-12_2"); strip a trailing _<digits>
    return re.sub(r"_\d+$", "", label)

def mesh_of(actor):
    comp = actor.get_component_by_class(unreal.StaticMeshComponent)
    return comp.static_mesh if comp else None

doors  = load_csv("Doors.csv", "Code")
lights = load_csv("Lights.csv", "Name")
found_doors, found_lights = set(), set()

with unreal.ScopedEditorTransaction("SMHQ autobuild"):
    for actor in actor_sub.get_all_level_actors():
        if not isinstance(actor, unreal.StaticMeshActor):
            continue
        label = base_label(actor.get_actor_label())

        # ---- Doors (only codes listed in Doors.csv — never by "D-" prefix)
        if label in doors:
            row, mesh = doors[label], mesh_of(actor)
            if not mesh:
                continue
            door = actor_sub.spawn_actor_from_class(DOOR_BP, actor.get_actor_location(), actor.get_actor_rotation())
            door.set_actor_label("BP_Door_" + label)
            door.set_actor_scale3d(actor.get_actor_scale3d())
            door.get_editor_property("DoorMesh").set_static_mesh(mesh)
            door.set_editor_property("IsSliding", row["Type"].strip().lower() == "sliding")
            door.set_editor_property("OpenAngle", float(row["OpenAngle"] or 90))
            door.set_editor_property("AutoOpen", yes(row["AutoOpen"]))
            door.set_editor_property("IsLocked", yes(row["Locked"]))
            door.tags = [unreal.Name(row["Space"].strip())]
            actor.set_actor_hidden_in_game(True)          # hide source leaf, keep it (DirectLink-safe)
            actor.set_is_temporarily_hidden_in_editor(True)
            found_doors.add(label)

        # ---- Lights (fixture names from Lights.csv)
        elif label in lights:
            row = lights[label]
            cls = LIGHT_CLASS.get(row["Kind"].strip().upper(), unreal.SpotLight)
            origin, extent = actor.get_actor_bounds(False)
            loc = unreal.Vector(origin.x, origin.y, origin.z - extent.z - 2.0)   # 2 cm below fixture
            # unreal.Rotator(roll, pitch, yaw): pitch -90 points spot/rect lights down
            rot = unreal.Rotator(0.0, -90.0, 0.0) if cls is not unreal.PointLight else unreal.Rotator()
            light = actor_sub.spawn_actor_from_class(cls, loc, rot)
            light.set_actor_label("L_" + label)
            lc = light.light_component
            lc.set_editor_property("intensity_units", unreal.LightUnits.LUMENS)
            lc.set_editor_property("intensity", float(row["Lumens"] or 800))
            lc.set_editor_property("use_temperature", True)
            lc.set_editor_property("temperature", float(row["Kelvin"] or 3000))
            if row.get("IES"):
                ies = unreal.EditorAssetLibrary.load_asset(row["IES"].strip())
                if ies:
                    lc.set_editor_property("ies_texture", ies)
                    lc.set_editor_property("use_ies_brightness", True)
            light.tags = [unreal.Name(row["Space"].strip())]      # BP_RoomLights collects by Space tag
            light.attach_to_actor(actor, "", unreal.AttachmentRule.KEEP_WORLD,
                                  unreal.AttachmentRule.KEEP_WORLD, unreal.AttachmentRule.KEEP_WORLD, False)
            found_lights.add(label)

# ---- QA report (feeds Check 1)
unreal.log(f"SMHQ autobuild: {len(found_doors)}/{len(doors)} doors, {len(found_lights)}/{len(lights)} lights")
for code in sorted(set(doors) - found_doors):
    unreal.log_warning(f"Door in Doors.csv not found in level: {code}")
for name in sorted(set(lights) - found_lights):
    unreal.log_warning(f"Light in Lights.csv not found in level: {name}")
```

Notes:
- If Datasmith imports the **definition** name instead of the instance name, the QA report will list every door as missing — fix naming in SketchUp, do not rename in Unreal.
- Re-running creates duplicates: delete `BP_Door_*` and `L_*` actors first (or extend the script to skip existing labels).
- Lumens/kelvin come from `Lights.csv`; IES column takes an imported IES asset path (e.g. `/Game/SMHQ/IES/Brand_Model`).

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
Vars (Instance Editable): OpenAngle=90, AutoOpen=false, IsSliding=false, SlideDistance=100, IsLocked=false; IsOpen (private)
Timeline TL_Door: float Alpha 0→1 over 1.2 s (auto tangents)
  Update: IsSliding ? DoorMesh.SetRelativeLocation((Lerp(0,SlideDistance,Alpha),0,0))
                    : DoorMesh.SetRelativeRotation((0,0,Lerp(0,OpenAngle,Alpha)))   // yaw
Event Interact: IsLocked ? PlaySound(Locked) : (IsOpen ? Reverse : Play); IsOpen = !IsOpen
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
Vars: ProjectCode, Title, Description, Images(Array<Texture2D>), Video(FileMediaSource) — or RowName into DT_Projects
OnBeginOverlap(player) → show prompt "Press E"
Event Interact → CreateWidget(WBP_InfoPanel) → set data → AddToViewport → SetInputMode GameAndUI, ShowMouseCursor
OnEndOverlap(player) → RemoveFromParent
```
`DT_Projects` row struct `S_Project { Code, Title, Description, Images[], Video }`, imported from CSV.

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
- Datasmith DirectLink — <https://dev.epicgames.com/documentation/en-us/unreal-engine/datasmith-directlink-in-unreal-engine>
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

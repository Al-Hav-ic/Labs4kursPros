# ParalelLab1 — лабораторна №1 (C++)

**Каталог:** `D:\KompSC\Edu\Cpp_codes\ParalelLab1`

Кожен пункт методички — **окремий** `.cpp` і **окремий** проєкт у Visual Studio.

| № | Файл | Пункт |
|---|------|--------|
| 1 | `lab1_2_1.cpp` | 1.2.1 — потоки без join/detach |
| 2 | `lab1_2_2.cpp` | 1.2.2 — detach |
| 3 | `lab1_2_3.cpp` | 1.2.3 — список без м'ютекса |
| 4 | `lab1_2_4.cpp` | 1.2.4 — lock/unlock |
| 5 | `lab1_2_5.cpp` | 1.2.5 — lock_guard |
| 6 | `lab1_2_6.cpp` | 1.2.6 — exchangePerson |
| 7 | `lab1_2_7.cpp` | 1.2.7 — unique_lock |

Спільні файли: `console_utf8.hpp`, `utf8.manifest`, `theory.html`, `PLAN.md`.

---

## Visual Studio — запуск одного файлу

1. Відкрити **`ParalelLab1.sln`**.
2. У **Solution Explorer** ПКМ на потрібний проєкт (`lab1_2_3` тощо) → **Set as Startup Project** (жирним стане лише один).
3. Зверху: **x64**, **Debug**.
4. **Ctrl+F5** — запуск без відладки.

Збірка лише поточного проєкту: ПКМ на проєкт → **Build**.  
Готовий `.exe`: `bin\lab1_2_3\Debug\lab1_2_3.exe` (після збірки).

---

## Командний рядок — один пункт без Visual Studio

```text
cd /d D:\KompSC\Edu\Cpp_codes\ParalelLab1
run.bat 3
```

Або лише збірка:

```text
build.bat 3
bin\lab1_2_3\Debug\lab1_2_3.exe
```

Усі пункти:

```text
build.bat all
```

Потрібен **Visual Studio 2022** з компонентом «Розробка класичних програм на C++» (компілятор `cl`).

---

## Кирилиця

У проєктах увімкнено UTF-8 (`/utf-8`, маніфест, `setup_console_utf8()` у пунктах 3–7). Після змін — **Rebuild** поточного проєкту.

---

## Стара папка на Desktop

Робоча копія — **тільки** `D:\KompSC\Edu\Cpp_codes\ParalelLab1`. Каталог `Desktop\Paralel` можна не використовувати.

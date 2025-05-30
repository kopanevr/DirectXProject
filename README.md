# **DIRECTX PROJECT**

![DirectX Version](https://img.shields.io/badge/DirectX-11-blue)

## ПРЕДИСЛОВИЕ

Данный проект представляет собой пример реализации рендеринга текстуры на треугольнике с применением [DirectX Api](https://www.microsoft.com/en-us/download/details.aspx?id=17431). Для отрисовки элементов графического интерфейса использовалась библиотека [Dear ImGui](https://github.com/ocornut/imgui).

## ОПИСАНИЕ

Результат работы приложения представлен на изображениях:

ПРИМЕР 1

|  |
| :--: |
| **Пример 1** |
| ![1](/doc/images/1.PNG) |

ПРИМЕР 2

Пример с другой текстурой.

|  |
| :--: |
| **Пример 2** |
| ![2](/doc/images/2.PNG) |

ПРИМЕР 3

Добавлен функционал для установки координат положения треугольника.

|  |
| :--: |
| **Пример 3** |
| ![3](/doc/images/3.PNG) |

ПРИМЕР 4

Пример с другой текстурой.

|  |
| :--: |
| **Пример 4** |
| ![4](/doc/images/4.PNG) |

ПРИМЕР 5

Добавлен функционал для установки угла поворота.

|  |
| :--: |
| **Пример 5** |
| ![5](/doc/images/5.PNG) |

Начальные координаты вершин треугольник имеет следующие:

```cpp
static Vertex vertices[] = {
//                           u      v
    { 0.0f,  0.5f,  0.0f,    0.5f,  0.0f },
    { 0.5f, -0.5f,  0.0f,    1.0f,  1.0f },
    {-0.5f, -0.5f,  0.0f,    0.0f,  1.0f }
};
```

## ЗАКЛЮЧЕНИЕ

## ССЫЛКИ

* [Windows Api](https://learn.microsoft.com/en-us/windows/win32/api) - Windows API.
* [DirectX Api](https://www.microsoft.com/en-us/download/details.aspx?id=17431) - DirectX API.
* [Dear ImGui](https://github.com/ocornut/imgui) - Библиотека для создания графических интерфейсов.

# **DIRECTX PROJECT**

![DirectX Version](https://img.shields.io/badge/DirectX-11-blue)

## ПРЕДИСЛОВИЕ

Данный проект представляет собой пример реализации рендеринга текстуры на треугольнике с применением [DirectX Api](https://www.microsoft.com/en-us/download/details.aspx?id=17431). Для отрисовки элементов графического интерфейса использовалась библиотека [Dear ImGui](https://github.com/ocornut/imgui).

## ОПИСАНИЕ

Результат работы приложения представлен на изображениях:

|  |
| :--: |
| **** |
| ![0](/doc/images/1.GIF) |

|  |
| :--: |
| **** |
| ![1](/doc/images/2.GIF) |

Добавлен функционал для установки угла поворота.

|  |
| :--: |
| **Пример 1** |
| ![2](/doc/images/1.PNG) |

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

Программа реализует класс Polynomial для работы с многочленами одной переменной (в данном случае y). Она позволяет:
  Считывать многочлен из файла
  Парсить его в упорядоченный односвязный список
  Выводить в канонической форме
  Сохранять результат обратно в файл

Структура проекта
  Файлы:

    Polynomial.h - заголовочный файл с реализацией классов
    main.cpp - главный исполняемый файл

Ключевые компоненты

  1. Класс Node Представляет узел односвязного списка:
    Coefficient - коэффициент при одночлене
    Power - степень одночлена
    Next - указатель на следующий узел

  2. Класс Polynomial Основной класс для работы с многочленами:
    Head - указатель на начало списка
  
  Приватные методы:
    CleanString() - удаляет пробелы и заменяет тире
    InsertTerm() - вставляет одночлен с сортировкой по убыванию степени
    
  Публичные методы:
    ParseFromString() - парсит строку в многочлен
    ToString() - возвращает строковое представление
    LoadFromFile() - загружает многочлен из файла
    AppendToFile() - дописывает результат в файл

Особенности работы

  Формат входных данных:

    Допустимые символы: +, -, цифры, y, ^

    Пример корректных выражений:

      -3y^2 + 5y - 2 + 7y^4 - 8y^3 + 2y^2
      5y+3-2y^4
      -y+7

    Автоматически обрабатывает:
      Пробелы
      Пропущенный коэффициент 1 (например, +y)
      Пропущенную степень 1 (например, -5y)

  Алгоритм работы:

    Удаление всех пробелов из входной строки
    Разделение на одночлены по знакам +/-
    Парсинг каждого одночлена:
      Определение знака
      Извлечение коэффициента
      Проверка переменной (должна быть y)
      Извлечение степени (по умолчанию 1)
    Упорядоченное добавление в список:
      Объединение одночленов с одинаковыми степенями
      Автоматическое удаление нулевых коэффициентов
      Сортировка по убыванию степеней

Формат результата:

  Без пробелов
  Одночлены упорядочены по убыванию степеней
  Пример: 7y^4-8y^3-y^2+5y-2


Инструкция по использованию
  1. Создайте текстовый файл с многочленом (например input.txt): "-3y^2 + 5y - 2 + 7y^4 - 8y^3 + 2y^2"
  2. Запустите программу
  3. Введите имя файла при запросе: "Введите имя файла: input.txt"
  4. Программа выведет:
     
       Исходный многочлен: -3y^2 + 5y - 2 + 7y^4 - 8y^3 + 2y^2
       Считанный многочлен: 7y^4-8y^3-y^2+5y-2
       Результат добавлен в файл.

  5.Файл input.txt будет дополнен: "Результат: 7y^4-8y^3-y^2+5y-2"

Обработка ошибок
  Программа обнаруживает:

    Пустые входные строки
    Некорректные одночлены
    Разные переменные в многочлене (например, x и y)
    Отсутствующие файлы
    Некорректные символы в степени
  
  Пример сообщения об ошибке: Ошибка: Некорректная переменная в: +5x


Мусин ИТ-13,14-2024

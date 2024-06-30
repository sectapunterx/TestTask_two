
### Эффективность торговой стратегии

## Описание

Этот проект включает два компонента: скрипт на Python для анализа торговых стратегий на основе 19 метрик и программу на C++ для генерации данных. Python-скрипт считывает данные из CSV файла, вычисляет оценку каждой метрики для каждой стратегии, и записывает результаты в выходной CSV файл. Также он выводит результаты анализа в консоль, включая оценки GOOD, MEDIUM и BAD, а также количество метрик каждого типа. В конце выводится информация о наилучшей и наихудшей стратегиях.

## Установка

1. Клонируйте репозиторий себе на компьютер.

2. Установите необходимые для работы библиотеки:
      ```bash
      pip install pandas
      ```

## Использование

Для запуска программы выполните следующие шаги:

1. Скомпилируйте и запустите программу на C++ для генерации данных:
      ```bash
      cd cpp
      g++ -o main main.cpp
      ./main ../output_data.csv
      cd ..
      ```

2. Запустите программу на Python для анализа метрик:
    ```bash
    cd python
    python main.py ../output_data.csv ../results_py.csv
    cd ..
    ```

`output_data.csv` - файл, являющийся результатом работы С++ скрипта. 

`results_py.csv` - файл, являющийся результатом работы Python скрипта. 

## Структура данных

CSV файл должен содержать строки с метриками для каждой стратегии. Пример:
```csv
Max Drawdown,Average Duration,Max Duration,Recovery Factor,Sharpe Ratio,Sortino Ratio,Calmar Ratio,Profit Factor,Win Rate,Loss Rate,Average Win,Average Loss,Max Consecutive Wins,Max Consecutive Losses,Expectancy,Volatility,Annualized Return,Trading Frequency,Turnover Rate,Total Profit
500,300,800,1.2,1.1,1.3,1.0,1.8,0.7,0.3,500,300,10,2,1000,200,0.15,15,2,1500
...
```

#### Диапазоны метрик

| Метрика               | GOOD                    | MEDIUM                  | BAD                |
|-----------------------|-------------------------|-------------------------|--------------------|
| Max Drawdown          | 0 - 1000                | 1000 - 3000             | > 3000             |
| Average Duration      | 50 - 400                | 400 - 600               | > 600              |
| Max Duration          | 100 - 1000              | 1000 - 1500             | > 1500             |
| Recovery Factor       | 1 - ∞                   | 0.5 - 1                 | < 0.5              |
| Sharpe Ratio          | 1 - ∞                   | 0.5 - 1                 | < 0.5              |
| Sortino Ratio         | 1 - ∞                   | 0.5 - 1                 | < 0.5              |
| Calmar Ratio          | 1 - ∞                   | 0.5 - 1                 | < 0.5              |
| Profit Factor         | 1.5 - ∞                 | 1 - 1.5                 | < 1                |
| Win Rate              | 0.6 - 1                 | 0.5 - 0.6               | < 0.5              |
| Loss Rate             | 0 - 0.4                 | 0.4 - 0.5               | > 0.5              |
| Average Win           | 0 - ∞                   | 0 - 0                   | < 0                |
| Average Loss          | -∞ - 0                  | 0 - 0                   | > 0                |
| Max Consecutive Wins  | 1 - ∞                   | 0 - 0                   | < 1                |
| Max Consecutive Losses| 0 - 3                   | 3 - 5                   | > 5                |
| Expectancy            | 0 - ∞                   | 0 - 0                   | < 0                |
| Volatility            | 0 - 500                 | 500 - 1000              | > 1000             |
| Annualized Return     | 0.1 - ∞                 | 0 - 0.1                 | < 0                |
| Trading Frequency     | 10 - ∞                  | 5 - 10                  | < 5                |
| Turnover Rate         | 0 - ∞                   | 0 - 0                   | < 0                |
| Total Profit          | 1000 - ∞                | 0 - 1000                | < 0                |


- **Max Drawdown**. Максимальная просадка: разница между локальным максимумом и текущим кумулятивным профитом. Ключевая метрика.
- **Average Duration**. Средняя длительность сделок. Вторичная метрика.
- **Max Duration**. Максимальная длительность сделки. Вторичная метрика.
- **Recovery Factor**. Отношение итоговой прибыли к максимальной просадке. Ключевая метрика.
- **Sharpe Ratio**. Средняя прибыль на единицу риска. Ключевая метрика.
- **Sortino Ratio**. Похоже на Sharpe Ratio, но учитывает только негативные колебания прибыли. Ключевая метрика.
- **Calmar Ratio**. Отношение средней годовой доходности к максимальной просадке. Ключевая метрика.
- **Profit Factor**. Отношение общей прибыли к общим убыткам. Ключевая метрика.
- **Win Rate**. Процент выигрышных сделок. Ключевая метрика.
- **Loss Rate**. Процент убыточных сделок. Вторичная метрика.
- **Average Win**. Средний размер выигрыша. Вторичная метрика.
- **Average Loss**. Средний размер убытка. Вторичная метрика.
- **Max Consecutive Wins**. Максимальное количество подряд идущих выигрышных сделок. Вторичная метрика.
- **Max Consecutive Losses**. Максимальное количество подряд идущих убыточных сделок. Вторичная метрика.
- **Expectancy**. Средний доход на сделку. Ключевая метрика.
- **Volatility**. Измеряет колебания прибыли. Вторичная метрика.
- **Annualized Return**. Годовая доходность. Ключевая метрика.
- **Trading Frequency**. Количество сделок за период. Вторичная метрика.
- **Turnover Rate**. Оборот по отношению к общему объему рынка. Вторичная метрика.
- **Total Profit**. Общая прибыль. Ключевая метрика.
## Логика оценки

Для каждой метрики каждой стратегии вычисляется оценка:
- GOOD: Значение метрики попадает в диапазон GOOD
- MEDIUM: Значение метрики попадает в диапазон MEDIUM
- BAD: Значение метрики попадает в диапазон BAD

## Оценка пригодности стратегии (Suitability)

Оценка стратегии основывается на проценте GOOD и BAD метрик:
- Suitable for real market deployment (GOOD): Если процент GOOD метрик > 70%
- May be considered for real market deployment (MEDIUM): Если процент GOOD метрик < 70% и процент BAD метрик < 30%
- Not suitable for real market deployment (BAD): В остальных случаях

## Вывод

Программа выводит результаты анализа в консоль в следующем формате:
```
Strategy: 1
Max Drawdown: GOOD
Average Duration: MEDIUM
...
Suitability: Suitable for real market deployment (GOOD)
Good - 75.00%, Medium - 15.00%, Bad - 10.00%, bad metrics: 2
Suitability Score: 80.00%
Number of GOOD metrics: 15, MEDIUM metrics: 3, BAD metrics: 2

Best strategy: 1. Good - 75.00%, Medium - 15.00%, Bad - 10.00%, bad metrics: 2
Worst strategy: 3. Good - 35.00%, Medium - 30.00%, Bad - 35.00%, bad metrics: 7
```

## Расчет Suitability Score

`Suitability Score` - это оценка пригодности стратегии для реальной торговли. Она рассчитывается на основе количества метрик, которые попадают в категории GOOD, MEDIUM и BAD. Для каждой метрики рассчитываются следующие категории:

- **GOOD**: Значение метрики попадает в хороший диапазон.
- **MEDIUM**: Значение метрики попадает в средний диапазон.
- **BAD**: Значение метрики попадает в плохой диапазон.

Каждой категории присваивается вес:
- GOOD: вес 1
- MEDIUM: вес 0.5
- BAD: вес 0

Оценка пригодности (`Suitability Score`) рассчитывается как взвешенное среднее:

$$ \text{Suitability Score} = \frac{\text{GOOD Count} + 0.5 \cdot \text{MEDIUM Count}}{\text{Total Number of Metrics}} \cdot 100 $$

где:
- GOOD Count - количество метрик, которые попали в категорию GOOD,
- MEDIUM Count - количество метрик, которые попали в категорию MEDIUM,
- Total Number of Metrics - общее количество метрик (в нашем случае 19).

## Вывод в файл

Результаты анализа также сохраняются в выходной CSV файл в следующем формате:
```csv
Max Drawdown,Average Duration,Max Duration,Recovery Factor,Sharpe Ratio,Sortino Ratio,Calmar Ratio,Profit Factor,Win Rate,Loss Rate,Average Win,Average Loss,Max Consecutive Wins,Max Consecutive Losses,Expectancy,Volatility,Annualized Return,Trading Frequency,Turnover Rate,Total Profit,Suitability
GOOD,MEDIUM,GOOD,GOOD,GOOD,GOOD,GOOD,GOOD,GOOD,BAD,GOOD,BAD,GOOD,BAD,GOOD,GOOD,GOOD,GOOD,GOOD,GOOD,Suitable for real market deployment (GOOD)
...
```

import sys
import pandas as pd

# Диапазоны для метрик
good_ranges = {
    "Max Drawdown": (0, 1000),
    "Average Duration": (50, 400),
    "Max Duration": (100, 1000),
    "Recovery Factor": (1, float('inf')),
    "Sharpe Ratio": (1, float('inf')),
    "Sortino Ratio": (1, float('inf')),
    "Calmar Ratio": (1, float('inf')),
    "Profit Factor": (1.5, float('inf')),
    "Win Rate": (0.6, 1),
    "Loss Rate": (0, 0.4),
    "Average Win": (0, float('inf')),
    "Average Loss": (-float('inf'), 0),
    "Max Consecutive Wins": (1, float('inf')),
    "Max Consecutive Losses": (0, 3),
    "Expectancy": (0, float('inf')),
    "Volatility": (0, 500),
    "Annualized Return": (0.1, float('inf')),
    "Trading Frequency": (10, float('inf')),
    "Turnover Rate": (0, float('inf')),
    "Total Profit": (1000, float('inf'))
}

medium_ranges = {
    "Max Drawdown": (1000, 3000),
    "Average Duration": (400, 600),
    "Max Duration": (1000, 1500),
    "Recovery Factor": (0.5, 1),
    "Sharpe Ratio": (0.5, 1),
    "Sortino Ratio": (0.5, 1),
    "Calmar Ratio": (0.5, 1),
    "Profit Factor": (1, 1.5),
    "Win Rate": (0.5, 0.6),
    "Loss Rate": (0.4, 0.5),
    "Average Win": (0, 0),
    "Average Loss": (0, 0),
    "Max Consecutive Wins": (0, 0),
    "Max Consecutive Losses": (3, 5),
    "Expectancy": (0, 0),
    "Volatility": (500, 1000),
    "Annualized Return": (0, 0.1),
    "Trading Frequency": (5, 10),
    "Turnover Rate": (0, 0),
    "Total Profit": (0, 1000)
}


def evaluate_metric(metric, value):
    if good_ranges[metric][0] <= value <= good_ranges[metric][1]:
        return "GOOD"
    elif medium_ranges[metric][0] <= value <= medium_ranges[metric][1]:
        return "MEDIUM"
    else:
        return "BAD"


def analyze_metrics(input_file, output_file):
    df = pd.read_csv(input_file)
    evaluations = df.apply(lambda row: {metric: evaluate_metric(metric, row[metric]) for metric in df.columns}, axis=1)
    evaluation_df = pd.DataFrame(evaluations.tolist())
    evaluation_df.to_csv(output_file, index=False)

    # Оценка пригодности стратегии для реальной биржи
    suitability = []
    best_strategy = None
    worst_strategy = None
    best_score = -1
    worst_score = 101  # Максимальное возможное значение suitability_score + 1

    num_metrics = len(df.columns) - 1  # Учитываем одну колонку для 'Suitability'

    for index, row in evaluation_df.iterrows():
        good_count = sum(value == "GOOD" for value in row.values)
        medium_count = sum(value == "MEDIUM" for value in row.values)
        bad_count = sum(value == "BAD" for value in row.values)

        good_percentage = (good_count / num_metrics) * 100
        bad_percentage = (bad_count / num_metrics) * 100

        if good_percentage > 70:
            suitability.append("Suitable for real market deployment (GOOD)")
        elif good_percentage < 70 and bad_percentage < 30:
            suitability.append("May be considered for real market deployment (MEDIUM)")
        else:
            suitability.append("Not suitable for real market deployment (BAD)")

        suitability_score = (good_count + medium_count * 0.5) / num_metrics * 100

        if suitability_score > best_score:
            best_score = suitability_score
            best_strategy = (index + 1, good_count, medium_count, bad_count)

        if suitability_score < worst_score:
            worst_score = suitability_score
            worst_strategy = (index + 1, good_count, medium_count, bad_count)

    evaluation_df["Suitability"] = suitability

    evaluation_df.to_csv(output_file, index=False)

    for index, row in evaluation_df.iterrows():
        good_count = sum(value == "GOOD" for value in row.values)
        medium_count = sum(value == "MEDIUM" for value in row.values)
        bad_count = sum(value == "BAD" for value in row.values)

        good_percentage = (good_count / num_metrics) * 100
        medium_percentage = (medium_count / num_metrics) * 100
        bad_percentage = (bad_count / num_metrics) * 100
        suitability_score = (good_count + medium_count * 0.5) / num_metrics * 100

        print(f"Strategy: {index + 1}")
        for metric, status in row.items():
            if metric != "Suitability":
                print(f"{metric}: {status}")
        print(f"Suitability: {row['Suitability']}")
        print(
            f"Good - {good_percentage:.2f}%, Medium - {medium_percentage:.2f}%, Bad - {bad_percentage:.2f}%, bad metrics: {bad_count}")
        print(f"Suitability Score: {suitability_score:.2f}%")
        print(f"Number of GOOD metrics: {good_count}, MEDIUM metrics: {medium_count}, BAD metrics: {bad_count}")
        print()

    if best_strategy:
        best_index, good_count, medium_count, bad_count = best_strategy
        good_percentage = (good_count / num_metrics) * 100
        medium_percentage = (medium_count / num_metrics) * 100
        bad_percentage = (bad_count / num_metrics) * 100
        print(
            f"Best strategy: {best_index}. Good - {good_percentage:.2f}%, Medium - {medium_percentage:.2f}%, Bad - {bad_percentage:.2f}%, bad metrics: {bad_count}")

    if worst_strategy:
        worst_index, good_count, medium_count, bad_count = worst_strategy
        good_percentage = (good_count / num_metrics) * 100
        medium_percentage = (medium_count / num_metrics) * 100
        bad_percentage = (bad_count / num_metrics) * 100
        print(
            f"Worst strategy: {worst_index}. Good - {good_percentage:.2f}%, Medium - {medium_percentage:.2f}%, Bad - {bad_percentage:.2f}%, bad metrics: {bad_count}")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python main.py <input_file> <output_file>")
    else:
        input_file = sys.argv[1]
        output_file = sys.argv[2]
        analyze_metrics(input_file, output_file)

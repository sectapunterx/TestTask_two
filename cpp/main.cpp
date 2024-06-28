#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <limits>
#include <tuple>
#include <fstream>
#include <iterator>
#include <string>
#include <sstream>

// Константы для генерации случайных данных
const int MIN_ROWS = 100;
const int MAX_ROW_LENGTH = 1000;
const int MIN_ROW_LENGTH = 1;
const int MAX_PROFIT = 1000;
const int MIN_PROFIT = -1000;
const int MAX_DURATION = 1200;
const int MIN_DURATION = 1;

struct Trade {
    double profit;
    double duration;
};

using TradeRow = std::vector<Trade>;

TradeRow generate_random_row(std::mt19937 &rng) {
  std::uniform_int_distribution<int> length_dist(MIN_ROW_LENGTH, MAX_ROW_LENGTH);
  std::uniform_real_distribution<double> profit_dist(MIN_PROFIT, MAX_PROFIT);
  std::uniform_real_distribution<double> duration_dist(MIN_DURATION, MAX_DURATION);

  int length = length_dist(rng);
  TradeRow row(length);
  for (int i = 0; i < length; ++i) {
    row[i] = {profit_dist(rng), duration_dist(rng)};
  }
  return row;
}

double calculate_sharpe_ratio(const std::vector<double>& profits) {
  double mean_profit = std::accumulate(profits.begin(), profits.end(), 0.0) / profits.size();
  double profit_variance = std::inner_product(profits.begin(), profits.end(), profits.begin(), 0.0) / profits.size() - mean_profit * mean_profit;
  double profit_stddev = std::sqrt(profit_variance);
  return profit_stddev == 0 ? std::numeric_limits<double>::infinity() : mean_profit / profit_stddev;
}

double calculate_sortino_ratio(const std::vector<double>& profits) {
  double mean_profit = std::accumulate(profits.begin(), profits.end(), 0.0) / profits.size();
  double negative_variance = 0.0;
  int negative_count = 0;
  for (double profit : profits) {
    if (profit < 0) {
      negative_variance += profit * profit;
      negative_count++;
    }
  }
  negative_variance /= profits.size();
  double negative_stddev = std::sqrt(negative_variance);
  return negative_stddev == 0 ? std::numeric_limits<double>::infinity() : mean_profit / negative_stddev;
}

double calculate_calmar_ratio(double mean_profit, double max_drawdown) {
  return max_drawdown == 0 ? std::numeric_limits<double>::infinity() : mean_profit / max_drawdown;
}

double calculate_profit_factor(const std::vector<double>& profits) {
  double total_positive_profit = std::accumulate(profits.begin(), profits.end(), 0.0, [](double sum, double profit) {
      return profit > 0 ? sum + profit : sum;
  });
  double total_negative_profit = std::accumulate(profits.begin(), profits.end(), 0.0, [](double sum, double profit) {
      return profit < 0 ? sum + profit : sum;
  });
  return total_negative_profit == 0 ? std::numeric_limits<double>::infinity() : total_positive_profit / std::abs(total_negative_profit);
}

double calculate_win_rate(const std::vector<double>& profits) {
  int win_count = std::count_if(profits.begin(), profits.end(), [](double profit) {
      return profit > 0;
  });
  return static_cast<double>(win_count) / profits.size();
}

double calculate_loss_rate(const std::vector<double>& profits) {
  int loss_count = std::count_if(profits.begin(), profits.end(), [](double profit) {
      return profit < 0;
  });
  return static_cast<double>(loss_count) / profits.size();
}

double calculate_average_win(const std::vector<double>& profits) {
  double total_positive_profit = std::accumulate(profits.begin(), profits.end(), 0.0, [](double sum, double profit) {
      return profit > 0 ? sum + profit : sum;
  });
  int win_count = std::count_if(profits.begin(), profits.end(), [](double profit) {
      return profit > 0;
  });
  return win_count == 0 ? 0 : total_positive_profit / win_count;
}

double calculate_average_loss(const std::vector<double>& profits) {
  double total_negative_profit = std::accumulate(profits.begin(), profits.end(), 0.0, [](double sum, double profit) {
      return profit < 0 ? sum + profit : sum;
  });
  int loss_count = std::count_if(profits.begin(), profits.end(), [](double profit) {
      return profit < 0;
  });
  return loss_count == 0 ? 0 : total_negative_profit / loss_count;
}

double calculate_max_consecutive_wins(const std::vector<double>& profits) {
  int max_consecutive_wins = 0;
  int current_wins = 0;
  for (double profit : profits) {
    if (profit > 0) {
      current_wins++;
      if (current_wins > max_consecutive_wins) {
        max_consecutive_wins = current_wins;
      }
    } else {
      current_wins = 0;
    }
  }
  return max_consecutive_wins;
}

double calculate_max_consecutive_losses(const std::vector<double>& profits) {
  int max_consecutive_losses = 0;
  int current_losses = 0;
  for (double profit : profits) {
    if (profit < 0) {
      current_losses++;
      if (current_losses > max_consecutive_losses) {
        max_consecutive_losses = current_losses;
      }
    } else {
      current_losses = 0;
    }
  }
  return max_consecutive_losses;
}

double calculate_expectancy(const std::vector<double>& profits) {
  double total_profit = std::accumulate(profits.begin(), profits.end(), 0.0);
  return total_profit / profits.size();
}

double calculate_volatility(const std::vector<double>& profits) {
  double mean_profit = std::accumulate(profits.begin(), profits.end(), 0.0) / profits.size();
  double profit_variance = std::inner_product(profits.begin(), profits.end(), profits.begin(), 0.0) / profits.size() - mean_profit * mean_profit;
  return std::sqrt(profit_variance);
}

double calculate_annualized_return(const std::vector<double>& profits) {
  double mean_profit = std::accumulate(profits.begin(), profits.end(), 0.0) / profits.size();
  return mean_profit * 252;  // Assuming 252 trading days
}

double calculate_trading_frequency(const std::vector<double>& profits) {
  return profits.size();
}

double calculate_turnover_rate(int trading_frequency, double max_duration) {
  return trading_frequency / max_duration;  // Simplified
}

std::tuple<double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double>
calculate_metrics(const TradeRow &row) {
  double max_drawdown = 0;
  double max_drawdown_peak = 0;
  double total_duration = 0;
  double max_duration = 0;
  double cumulative_profit = 0;

  std::vector<double> cumulative_profits(row.size());
  std::vector<double> profits(row.size());
  for (size_t i = 0; i < row.size(); ++i) {
    cumulative_profit += row[i].profit;
    cumulative_profits[i] = cumulative_profit;
    profits[i] = row[i].profit;
    total_duration += row[i].duration;
    if (row[i].duration > max_duration) {
      max_duration = row[i].duration;
    }
    if (cumulative_profit > max_drawdown_peak) {
      max_drawdown_peak = cumulative_profit;
    }
    double drawdown = max_drawdown_peak - cumulative_profit;
    if (drawdown > max_drawdown) {
      max_drawdown = drawdown;
    }
  }

  double average_duration = row.empty() ? 0 : total_duration / row.size();
  double recovery_factor = max_drawdown == 0 ? std::numeric_limits<double>::infinity() : cumulative_profit / max_drawdown;
  double sharpe_ratio = calculate_sharpe_ratio(profits);
  double sortino_ratio = calculate_sortino_ratio(profits);
  double calmar_ratio = calculate_calmar_ratio(cumulative_profit / row.size(), max_drawdown);
  double profit_factor = calculate_profit_factor(profits);
  double win_rate = calculate_win_rate(profits);
  double loss_rate = calculate_loss_rate(profits);
  double average_win = calculate_average_win(profits);
  double average_loss = calculate_average_loss(profits);
  double max_consecutive_wins = calculate_max_consecutive_wins(profits);
  double max_consecutive_losses = calculate_max_consecutive_losses(profits);
  double expectancy = calculate_expectancy(profits);
  double volatility = calculate_volatility(profits);
  double annualized_return = calculate_annualized_return(profits);
  double trading_frequency = calculate_trading_frequency(profits);
  double turnover_rate = calculate_turnover_rate(trading_frequency, max_duration);

  return {max_drawdown, average_duration, max_duration, recovery_factor, sharpe_ratio, sortino_ratio, calmar_ratio, profit_factor,
          win_rate, loss_rate, average_win, average_loss, max_consecutive_wins, max_consecutive_losses, expectancy, volatility,
          annualized_return, trading_frequency, turnover_rate, cumulative_profit};
}

void write_metrics_to_file(const std::string& file_path, const std::vector<std::tuple<double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double>>& metrics) {
  std::ofstream file(file_path);
  if (!file) {
    std::cerr << "Error: Could not open file " << file_path << " for writing." << std::endl;
    return;
  }

  file << "Max Drawdown,Average Duration,Max Duration,Recovery Factor,Sharpe Ratio,Sortino Ratio,Calmar Ratio,Profit Factor,"
       << "Win Rate,Loss Rate,Average Win,Average Loss,Max Consecutive Wins,Max Consecutive Losses,Expectancy,Volatility,"
       << "Annualized Return,Trading Frequency,Turnover Rate,Total Profit\n";

  for (const auto& metric : metrics) {
    file << std::get<0>(metric) << "," << std::get<1>(metric) << "," << std::get<2>(metric) << "," << std::get<3>(metric) << ","
         << std::get<4>(metric) << "," << std::get<5>(metric) << "," << std::get<6>(metric) << "," << std::get<7>(metric) << ","
         << std::get<8>(metric) << "," << std::get<9>(metric) << "," << std::get<10>(metric) << "," << std::get<11>(metric) << ","
         << std::get<12>(metric) << "," << std::get<13>(metric) << "," << std::get<14>(metric) << "," << std::get<15>(metric) << ","
         << std::get<16>(metric) << "," << std::get<17>(metric) << "," << std::get<18>(metric) << "," << std::get<19>(metric) << "\n";
  }

  file.close();
}

int main(int argc, char* argv[]) {
  // Проверка количества аргументов
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <output_file_path>" << std::endl;
    return 1;
  }

  std::string output_file_path = argv[1];

  std::random_device rd;
  std::mt19937 rng(rd());

  std::vector<TradeRow> trade_rows(MIN_ROWS);
  for (int i = 0; i < MIN_ROWS; ++i) {
    trade_rows[i] = generate_random_row(rng);
  }

  std::vector<std::tuple<double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double, double>> metrics;
  for (const auto& row : trade_rows) {
    metrics.push_back(calculate_metrics(row));
  }

  write_metrics_to_file(output_file_path, metrics);

  return 0;
}

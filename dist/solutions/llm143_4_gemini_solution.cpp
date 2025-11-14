#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <random>
#include <chrono>

// Represents a single playing card.
struct Card {
    int suit;
    int value; // 1-13 for 2-A

    bool operator<(const Card& other) const {
        if (value != other.value) return value < other.value;
        return suit < other.suit;
    }
     bool operator==(const Card& other) const {
        return suit == other.suit && value == other.value;
    }
};

// Represents the evaluated strength of a 5-card hand.
struct HandValue {
    int rank; // 9: Straight Flush, 8: Four of a Kind, ..., 1: High Card
    std::vector<int> tiebreakers;

    bool operator<(const HandValue& other) const {
        if (rank != other.rank) return rank < other.rank;
        return tiebreakers < other.tiebreakers;
    }
};

// Evaluates a 5-card hand and returns its rank and tiebreaker values.
HandValue evaluate_5_cards(std::vector<Card>& hand) {
    std::map<int, int> value_counts;
    std::map<int, int> suit_counts;
    std::vector<int> values;
    for (const auto& card : hand) {
        value_counts[card.value]++;
        suit_counts[card.suit]++;
        values.push_back(card.value);
    }
    std::sort(values.begin(), values.end());

    bool is_flush = false;
    for (const auto& pair : suit_counts) {
        if (pair.second >= 5) {
            is_flush = true;
            break;
        }
    }

    bool is_straight = false;
    int straight_high_val = 0;
    std::vector<int> unique_values;
    if (!values.empty()) {
        unique_values.push_back(values[0]);
        for (size_t i = 1; i < values.size(); ++i) {
            if (values[i] != values[i-1]) {
                unique_values.push_back(values[i]);
            }
        }
    }

    if (unique_values.size() >= 5) {
        for (int i = unique_values.size() - 1; i >= 4; --i) {
            if (unique_values[i] - unique_values[i-4] == 4) {
                is_straight = true;
                straight_high_val = unique_values[i];
                break;
            }
        }
    }
    // Ace-low straight check: A,2,3,4,5 -> values 13,1,2,3,4
    bool is_ace_low_straight = unique_values.size() >= 5 &&
                               unique_values[0] == 1 && unique_values[1] == 2 &&
                               unique_values[2] == 3 && unique_values[3] == 4 &&
                               unique_values.back() == 13;
    if (!is_straight && is_ace_low_straight) {
        is_straight = true;
        straight_high_val = 4; // High card is 5 (value 4)
    }

    HandValue result;

    if (is_straight && is_flush) {
        result.rank = 9;
        result.tiebreakers.push_back(straight_high_val);
        return result;
    }

    std::vector<std::pair<int, int>> counts; // {count, value}
    for (auto const& [val, num] : value_counts) {
        counts.push_back({num, val});
    }
    std::sort(counts.begin(), counts.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first > b.first;
        return a.second > b.second;
    });
    
    std::vector<int> tiebreaker_vals;
    for(const auto& p : counts) tiebreaker_vals.push_back(p.second);

    if (counts[0].first == 4) {
        result.rank = 8;
        result.tiebreakers = tiebreaker_vals;
        return result;
    }

    if (counts[0].first == 3 && counts.size() > 1 && counts[1].first >= 2) {
        result.rank = 7;
        result.tiebreakers = tiebreaker_vals;
        return result;
    }

    if (is_flush) {
        result.rank = 6;
        std::vector<int> flush_values;
        int flush_suit = -1;
        for(auto const& [s, c] : suit_counts) if (c >= 5) flush_suit = s;
        for(const auto& card : hand) if(card.suit == flush_suit) flush_values.push_back(card.value);
        std::sort(flush_values.rbegin(), flush_values.rend());
        result.tiebreakers.assign(flush_values.begin(), flush_values.begin()+5);
        return result;
    }

    if (is_straight) {
        result.rank = 5;
        result.tiebreakers.push_back(straight_high_val);
        return result;
    }

    if (counts[0].first == 3) {
        result.rank = 4;
        result.tiebreakers = tiebreaker_vals;
        return result;
    }

    if (counts[0].first == 2 && counts.size() > 1 && counts[1].first == 2) {
        result.rank = 3;
        result.tiebreakers = tiebreaker_vals;
        return result;
    }

    if (counts[0].first == 2) {
        result.rank = 2;
        result.tiebreakers = tiebreaker_vals;
        return result;
    }

    result.rank = 1;
    result.tiebreakers = tiebreaker_vals;
    return result;
}

// Finds the best 5-card hand from a set of 7 cards.
HandValue evaluate_7_cards(std::vector<Card>& all_cards) {
    HandValue best_hand = {{0}, {}};
    if (all_cards.size() != 7) return best_hand;

    std::vector<int> v(7);
    std::fill(v.begin() + 5, v.end(), 0);
    std::fill(v.begin(), v.begin() + 5, 1);

    do {
        std::vector<Card> current_hand;
        for (int i = 0; i < 7; ++i) {
            if (v[i]) {
                current_hand.push_back(all_cards[i]);
            }
        }
        HandValue current_value = evaluate_5_cards(current_hand);
        if (best_hand < current_value) {
            best_hand = current_value;
        }
    } while (std::prev_permutation(v.begin(), v.end()));

    return best_hand;
}

std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int G;
    std::cin >> G;
    if (G == -1) return 0;

    for (int h = 1; h <= G; ++h) {
        while (true) {
            std::string token;
            std::cin >> token;
            if (token == "-1" || token == "SCORE") return 0;
            
            int hand_idx, round, my_stack, opp_stack, pot, num_board_cards;
            std::cin >> hand_idx >> round >> my_stack >> opp_stack >> pot >> num_board_cards;
            
            std::vector<Card> my_cards(2);
            std::cin >> token >> my_cards[0].suit >> my_cards[0].value >> my_cards[1].suit >> my_cards[1].value;
            
            std::vector<Card> board_cards(num_board_cards);
            std::cin >> token;
            for (int i = 0; i < num_board_cards; ++i) {
                std::cin >> board_cards[i].suit >> board_cards[i].value;
            }

            double win_prob, draw_prob;
            int rate_samples = 0;
            if (round == 1) rate_samples = 120;
            else if (round == 2) rate_samples = 80;
            else if (round == 3) rate_samples = 50;
            else if (round == 4) rate_samples = 50;
            
            if (my_stack > 0 && rate_samples > 0) {
                std::cout << "RATE " << rate_samples << std::endl;
                std::cin >> token;
                if (token == "-1") return 0;
                std::cin >> win_prob >> draw_prob;
            } else {
                win_prob = 0.5; draw_prob = 0.0;
            }

            double my_equity = win_prob + draw_prob / 2.0;

            std::vector<Card> deck;
            for (int s = 0; s < 4; ++s) {
                for (int v = 1; v <= 13; ++v) {
                    deck.push_back({s, v});
                }
            }
            auto is_known = [&](const Card& c){
                if (c == my_cards[0] || c == my_cards[1]) return true;
                for(const auto& bc : board_cards) if (c == bc) return true;
                return false;
            };
            deck.erase(std::remove_if(deck.begin(), deck.end(), is_known), deck.end());
            
            std::vector<int> raise_options;
            if (my_stack > 0) {
                if (pot > 0) {
                   raise_options.push_back(std::max(1, (int)round(0.5 * pot)));
                   raise_options.push_back(std::max(1, (int)round(0.75 * pot)));
                   raise_options.push_back(std::max(1, (int)round(1.0 * pot)));
                }
                raise_options.push_back(my_stack);
            }
            std::sort(raise_options.begin(), raise_options.end());
            raise_options.erase(std::unique(raise_options.begin(), raise_options.end()), raise_options.end());

            long long best_raise = 0;
            double max_ev_gain = 0.0;

            int S = 40;
            int M = 100;
            
            for (int raise_amount : raise_options) {
                if (raise_amount > my_stack) continue;
                if (raise_amount <= 0) continue;

                double pot_odds = (double)raise_amount / (pot + 2.0 * raise_amount);
                
                int folds = 0;
                int calls = 0;
                long long my_wins_when_called = 0;
                long long ties_when_called = 0;
                int num_rollouts_when_called = 0;

                for (int s = 0; s < S; ++s) {
                    std::shuffle(deck.begin(), deck.end(), rng);
                    std::vector<Card> opp_cards = {deck[0], deck[1]};
                    
                    long long opp_wins_sim = 0;
                    long long ties_sim = 0;
                    long long my_wins_sim = 0;

                    for (int m = 0; m < M; ++m) {
                        std::shuffle(deck.begin() + 2, deck.end(), rng);
                        std::vector<Card> current_board = board_cards;
                        int cards_to_deal = 5 - num_board_cards;
                        for(int i=0; i<cards_to_deal; ++i) current_board.push_back(deck[2+i]);
                        
                        std::vector<Card> my_7_cards = my_cards;
                        my_7_cards.insert(my_7_cards.end(), current_board.begin(), current_board.end());
                        std::vector<Card> opp_7_cards = opp_cards;
                        opp_7_cards.insert(opp_7_cards.end(), current_board.begin(), current_board.end());

                        HandValue my_hand_val = evaluate_7_cards(my_7_cards);
                        HandValue opp_hand_val = evaluate_7_cards(opp_7_cards);

                        if (opp_hand_val < my_hand_val) my_wins_sim++;
                        else if (my_hand_val < opp_hand_val) opp_wins_sim++;
                        else ties_sim++;
                    }
                    
                    double opp_equity = (opp_wins_sim + ties_sim / 2.0) / M;
                    
                    if (opp_equity > pot_odds) {
                        calls++;
                        my_wins_when_called += my_wins_sim;
                        ties_when_called += ties_sim;
                        num_rollouts_when_called += M;
                    } else {
                        folds++;
                    }
                }

                double p_fold = (S > 0) ? (double)folds / S : 0.0;
                double my_equity_vs_call_range = (calls > 0) ? (my_wins_when_called + ties_when_called / 2.0) / num_rollouts_when_called : 0.0;
                
                double ev_if_called = my_equity_vs_call_range * (pot + 2.0 * raise_amount) - raise_amount;
                double ev_raise = p_fold * pot + (1.0-p_fold) * ev_if_called;

                double ev_check = my_equity * pot;

                double ev_gain = ev_raise - ev_check;
                
                if (ev_gain > max_ev_gain) {
                    max_ev_gain = ev_gain;
                    best_raise = raise_amount;
                }
            }

            if (best_raise > 0) {
                std::cout << "ACTION RAISE " << best_raise << std::endl;
            } else {
                std::cout << "ACTION CHECK" << std::endl;
            }
            
            std::cin >> token;
            if (token == "-1") return 0;
            if (token == "RESULT") {
                double delta;
                std::cin >> delta;
                break;
            } else if(token == "OPP") {
                std::string opp_action;
                std::cin >> opp_action;
                if(opp_action == "FOLD") {
                    std::cin >> token; if(token=="-1") return 0; // RESULT
                    std::cin >> token; if(token=="-1") return 0; // delta
                    break;
                }
                if (round == 4) {
                    if (opp_action == "CALL") std::cin >> token; if(token=="-1") return 0; // consume raise amount
                    std::cin >> token; if(token=="-1") return 0; // RESULT
                    std::cin >> token; if(token=="-1") return 0; // delta
                    break;
                }
                if(opp_action == "CALL") {
                    long long amount;
                    std::cin >> amount;
                }
            }
        }
    }
    return 0;
}
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <random>
#include <chrono>
#include <set>

// Using 1-13 for 2-A
struct Card {
    int suit;
    int value;

    bool operator<(const Card& other) const {
        if (value != other.value) return value < other.value;
        return suit < other.suit;
    }
    bool operator==(const Card& other) const {
        return suit == other.suit && value == other.value;
    }
};

enum HandType {
    HIGH_CARD = 0,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH
};

struct HandStrength {
    HandType type;
    std::vector<int> tiebreakers;

    bool operator<(const HandStrength& other) const {
        if (type != other.type) return type < other.type;
        return tiebreakers < other.tiebreakers;
    }
};

namespace HandEvaluator {
    HandStrength evaluate_5_cards(std::vector<Card>& hand) {
        std::sort(hand.begin(), hand.end());

        bool is_flush = true;
        for (int i = 1; i < 5; ++i) {
            if (hand[i].suit != hand[0].suit) {
                is_flush = false;
                break;
            }
        }

        bool is_straight = false;
        bool is_ace_low_straight = false;
        std::vector<int> values;
        for(const auto& c: hand) values.push_back(c.value);
        
        bool unique_vals = true;
        for (size_t i = 1; i < values.size(); ++i) {
            if (values[i] == values[i-1]) {
                unique_vals = false;
                break;
            }
        }
        
        if (unique_vals) {
            if (values[0] == 1 && values[1] == 2 && values[2] == 3 && values[3] == 4 && values[4] == 13) {
                is_ace_low_straight = true;
                is_straight = true;
            } else if (values[4] - values[0] == 4) {
                is_straight = true;
            }
        }
        
        if (is_straight) {
            int high_card = is_ace_low_straight ? 4 : hand[4].value;
            if (is_flush) {
                return {STRAIGHT_FLUSH, {high_card}};
            }
            return {STRAIGHT, {high_card}};
        }

        if (is_flush) {
            std::vector<int> tiebreakers;
            for (int i = 4; i >= 0; --i) tiebreakers.push_back(hand[i].value);
            return {FLUSH, tiebreakers};
        }

        std::map<int, int> counts;
        for (const auto& card : hand) counts[card.value]++;
        
        std::vector<std::pair<int, int>> value_mult_pairs;
        for (const auto& card : hand) {
            value_mult_pairs.push_back({card.value, counts[card.value]});
        }
        
        std::sort(value_mult_pairs.begin(), value_mult_pairs.end(),
            [](const auto& a, const auto& b){
                if (a.second != b.second) return a.second > b.second;
                return a.first > b.first;
            });

        std::vector<int> tiebreakers;
        for (const auto& p : value_mult_pairs) {
            tiebreakers.push_back(p.first);
        }
        
        int max_mult = value_mult_pairs[0].second;
        if (max_mult == 4) return {FOUR_OF_A_KIND, tiebreakers};
        if (max_mult == 3) {
            if (value_mult_pairs[3].second == 2) return {FULL_HOUSE, tiebreakers};
            return {THREE_OF_A_KIND, tiebreakers};
        }
        if (max_mult == 2) {
            if (value_mult_pairs[2].second == 2) return {TWO_PAIR, tiebreakers};
            return {ONE_PAIR, tiebreakers};
        }
        return {HIGH_CARD, tiebreakers};
    }

    HandStrength evaluate_7_cards(std::vector<Card> seven_cards) {
        HandStrength best_strength = {HIGH_CARD, {0}};
        std::vector<bool> v(7);
        std::fill(v.begin(), v.begin() + 5, true);

        do {
            std::vector<Card> current_hand;
            for (int i = 0; i < 7; ++i) {
                if (v[i]) {
                    current_hand.push_back(seven_cards[i]);
                }
            }
            HandStrength current_strength = evaluate_5_cards(current_hand);
            if (best_strength < current_strength) {
                best_strength = current_strength;
            }
        } while (std::next_permutation(v.begin(), v.end()));
        
        return best_strength;
    }
}


class Bot {
public:
    Bot() {
        gen.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        for (int s = 0; s < 4; ++s) {
            for (int v = 1; v <= 13; ++v) {
                full_deck.push_back({s, v});
            }
        }
    }

    void run() {
        int G;
        std::cin >> G;
        if (G == -1) return;

        for (int i = 0; i < G; ++i) {
            play_hand();
        }
        
        std::string token;
        std::cin >> token;
        if (token == "SCORE") {
            std::string line;
            std::getline(std::cin, line);
        }
    }

private:
    std::mt19937 gen;
    std::vector<Card> full_deck;

    int h, r, a, b, P, k;
    std::vector<Card> my_cards;
    std::vector<Card> board;
    
    void play_hand() {
        while (true) {
            std::string token;
            std::cin >> token;
            if (token == "STATE") {
                parse_state();
                decide_action();
            } else if (token == "RESULT" || token == "SCORE") {
                std::string line;
                std::getline(std::cin, line);
                break;
            } else if (std::cin.eof() || token.empty()) {
                return;
            } else if (token == "-1") {
                exit(0);
            }
        }
    }

    void parse_state() {
        std::cin >> h >> r >> a >> b >> P >> k;
        std::string token;
        std::cin >> token; // ALICE
        my_cards.resize(2);
        std::cin >> my_cards[0].suit >> my_cards[0].value >> my_cards[1].suit >> my_cards[1].value;
        std::cin >> token; // BOARD
        board.resize(k);
        for (int i = 0; i < k; ++i) {
            std::cin >> board[i].suit >> board[i].value;
        }
    }

    double get_my_equity() {
        int samples;
        if (r == 1) samples = 40;
        else if (r == 2) samples = 60;
        else if (r == 3) samples = 80;
        else samples = 100;
        
        std::cout << "RATE " << samples << std::endl;
        std::string token;
        double w, d;
        std::cin >> token;
        if (token == "-1") exit(0);
        std::cin >> w >> d;
        return w + d / 2.0;
    }

    std::vector<Card> get_remaining_deck() {
        std::vector<Card> remaining;
        std::vector<Card> used_cards = my_cards;
        used_cards.insert(used_cards.end(), board.begin(), board.end());
        std::sort(used_cards.begin(), used_cards.end());

        for (const auto& card : full_deck) {
            if (!std::binary_search(used_cards.begin(), used_cards.end(), card)) {
                remaining.push_back(card);
            }
        }
        return remaining;
    }

    void decide_action() {
        double my_equity = get_my_equity();
        
        std::vector<Card> rem_deck = get_remaining_deck();
        std::vector<std::vector<Card>> bob_possible_hands;
        for (size_t i = 0; i < rem_deck.size(); ++i) {
            for (size_t j = i + 1; j < rem_deck.size(); ++j) {
                bob_possible_hands.push_back({rem_deck[i], rem_deck[j]});
            }
        }

        std::vector<double> bob_equities;
        bob_equities.reserve(bob_possible_hands.size());
        for (const auto& bob_hand : bob_possible_hands) {
            std::vector<Card> deck_for_sim;
            deck_for_sim.reserve(rem_deck.size() - 2);
            for(const auto& c : rem_deck) {
                if (!(c == bob_hand[0]) && !(c == bob_hand[1])) {
                    deck_for_sim.push_back(c);
                }
            }
            
            int wins = 0, ties = 0;
            const int N_MC_BOB = 20;

            for(int i=0; i<N_MC_BOB; ++i) {
                 std::shuffle(deck_for_sim.begin(), deck_for_sim.end(), gen);

                 std::vector<Card> opp_hand = {deck_for_sim[0], deck_for_sim[1]};
                 
                 std::vector<Card> bob_7_cards = bob_hand;
                 std::vector<Card> opp_7_cards = opp_hand;
                 std::vector<Card> final_board = board;
                 
                 int cards_to_deal = 5-board.size();
                 for(int j=0; j<cards_to_deal; ++j) {
                     final_board.push_back(deck_for_sim[j+2]);
                 }
                 bob_7_cards.insert(bob_7_cards.end(), final_board.begin(), final_board.end());
                 opp_7_cards.insert(opp_7_cards.end(), final_board.begin(), final_board.end());
                 
                 HandStrength s_bob = HandEvaluator::evaluate_7_cards(bob_7_cards);
                 HandStrength s_opp = HandEvaluator::evaluate_7_cards(opp_7_cards);

                 if (s_opp < s_bob) wins++;
                 else if (!(s_bob < s_opp)) ties++;
            }
            bob_equities.push_back((double)wins/N_MC_BOB + ((double)ties/N_MC_BOB)/2.0);
        }

        double ev_check = a + my_equity * P;
        
        double best_ev = ev_check;
        int best_action = 0;
        
        std::set<int> raise_candidates;
        raise_candidates.insert(1);
        if (a > 1) raise_candidates.insert(a);
        if (P / 2 >= 1 && P / 2 <= a) raise_candidates.insert(P/2);
        if (P >= 1 && P <= a) raise_candidates.insert(P);
        if (a / 2 >= 1 && a/2 <= a) raise_candidates.insert(a/2);

        for (double e : bob_equities) {
            if (e < 0.5 && e > 1e-9) {
                double x_crit_d = e * P / (1.0 - 2.0 * e);
                int x_crit = static_cast<int>(round(x_crit_d));
                if (x_crit >= 1 && x_crit <= a) raise_candidates.insert(x_crit);
                if (x_crit + 1 >= 1 && x_crit + 1 <= a) raise_candidates.insert(x_crit+1);
            }
        }
        
        for (int x : raise_candidates) {
            if (x < 1 || x > a) continue;

            double threshold = (double)x / (P + 2.0*x);
            int fold_count = 0;
            for (double e : bob_equities) {
                if (e <= threshold) {
                    fold_count++;
                }
            }
            double p_fold = (double)fold_count / bob_equities.size();

            double ev_raise = p_fold * (a + P) + (1.0 - p_fold) * (a - x + my_equity * (P + 2.0*x));
            
            if (ev_raise > best_ev) {
                best_ev = ev_raise;
                best_action = x;
            }
        }
        
        if (best_action > 0) {
            std::cout << "ACTION RAISE " << best_action << std::endl;
        } else {
            std::cout << "ACTION CHECK" << std::endl;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    Bot bot;
    bot.run();
    return 0;
}
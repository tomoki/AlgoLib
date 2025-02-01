######################
なんか最適化とか
######################

****************************************
焼き鈍し
****************************************

テスト中

.. code-block:: cpp

    // 焼きなまし法を行う、 まずは山登り法で試すこと
    // SCORE: スコアの型、 OPTIMIZE_TO_MAX_SCORE: スコアを最大化するなら true, 最小化するなら false、 HILL_CLIMBING: true なら山登り法
    template<typename SCORE, bool OPTIMIZE_TO_MAX_SCORE = true, bool HILL_CLIMBING = false>
    SCORE simulated_annealing(SCORE initial_score, // 初期スコア
                            const function<SCORE()>& update, // 更新しそのスコアを返す
                            const function<void()>& undo,    // 更新を取り消す
                            const SCORE start_temp, // 一度の遷移で動く最大幅
                            const SCORE end_temp,   // 一度の遷移で動く最小幅
                            const chrono::milliseconds annealing_time = chrono::milliseconds(1000))
    {
        assert(start_temp >= 0 && end_temp >= 0 && start_temp > end_temp);
        constexpr int NUMBER_OF_EVALUATE_PER_TIME_CHECK = 100;

        const auto start_time = chrono::system_clock::now();
        const auto end_time = start_time + annealing_time;

        auto current_time = start_time;
        auto current_score = initial_score;

        for (int annealing = 0; (annealing % NUMBER_OF_EVALUATE_PER_TIME_CHECK != 0) || ((current_time=chrono::system_clock::now()) < end_time); annealing++) {
            constexpr int PRECISION_FOR_FORCE_NEXT = 1000000;
            const auto progress = 1.0 * (current_time-start_time)/annealing_time;
            const auto current_temp = start_temp + (end_temp - start_temp) * progress;

            const SCORE updated_score = update();
            // スコアが悪くなっても遷移する確率
            const double probability_force_next = OPTIMIZE_TO_MAX_SCORE ?
                                                exp((updated_score - current_score) / current_temp) :
                                                exp(-(updated_score - current_score) / current_temp);
            const bool force_next = !HILL_CLIMBING && (probability_force_next * PRECISION_FOR_FORCE_NEXT) > (xor128() % PRECISION_FOR_FORCE_NEXT);
            const bool better_score = (OPTIMIZE_TO_MAX_SCORE ? (updated_score >= current_score) : (updated_score <= current_score));
            if(better_score || force_next){
                // Accept the change
                current_score = updated_score;
            } else {
                // undo change.
                undo();
            }
        }
        return current_score;
    }


.. code-block:: cpp

    void main() {
        const auto score = [](double x){
            // return (x-50) * (x-50);
            return max(-pow(-x+100,2)+100,-pow(-x+333,2)+333);
        };
        double current_x = 0;
        double prev_x = 0;
        const function<double()> update = [&]() {
            prev_x = current_x;
            current_x += ((double) (xor128() % 101)) - 50;
            return score(current_x);
        };
        const function<void()> undo = [&]() {
            current_x = prev_x;
        };
        double ans = simulated_annealing<double>(score(current_x), update, undo, 1000, 10);
        dump(current_x, ans);
    }
struct DigitDpCounter {
    string digits;
    int target_sum;
    long long memo[20][172][2];
    bool seen[20][172][2];

    long long dfs(int pos, int sum, bool tight, bool started) {
        if (sum > target_sum) return 0;
        if (pos == (int)digits.size()) {
            return sum == target_sum && (started || target_sum == 0);
        }
        if (!tight && seen[pos][sum][started]) {
            return memo[pos][sum][started];
        }

        int limit = tight ? digits[pos] - '0' : 9;
        long long answer = 0;

        for (int digit = 0; digit <= limit; ++digit) {
            bool next_started = started || digit != 0;
            int next_sum = sum + (next_started ? digit : 0);
            answer += dfs(pos + 1, next_sum, tight && digit == limit, next_started);
        }

        if (!tight) {
            seen[pos][sum][started] = true;
            memo[pos][sum][started] = answer;
        }
        return answer;
    }

    long long count_exact_sum(long long limit, int wanted_sum) {
        if (limit < 0) return 0;
        digits = to_string(limit);
        target_sum = wanted_sum;
        memset(seen, 0, sizeof(seen));
        return dfs(0, 0, true, false);
    }

    long long count_in_range(long long left, long long right, int wanted_sum) {
        return count_exact_sum(right, wanted_sum) - count_exact_sum(left - 1, wanted_sum);
    }
};

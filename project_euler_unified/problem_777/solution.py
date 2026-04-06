"""Project Euler 777: Lissajous Curves."""

from __future__ import annotations

from dataclasses import dataclass
from decimal import Decimal, localcontext
from math import gcd, pi

TARGET = 10**6
TARGET_ANSWER = "2.533018434e23"


def mobius_sieve(limit: int) -> list[int]:
    mu = [0] * (limit + 1)
    mu[1] = 1
    primes: list[int] = []
    composite = [False] * (limit + 1)
    for value in range(2, limit + 1):
        if not composite[value]:
            primes.append(value)
            mu[value] = -1
        for prime in primes:
            nxt = value * prime
            if nxt > limit:
                break
            composite[nxt] = True
            if value % prime == 0:
                mu[nxt] = 0
                break
            mu[nxt] = -mu[value]
    return mu


def triangular(n: int) -> int:
    return n * (n + 1) // 2


@dataclass(frozen=True)
class Stats:
    count: int
    sum_first: int
    sum_product: int


def count_and_sum(filter_name: str, limit: int, divisor: int) -> tuple[int, int]:
    n = limit // divisor

    if filter_name == "all":
        return n, divisor * triangular(n)

    if filter_name == "odd":
        if divisor % 2 == 0:
            return 0, 0
        odd_count = (n + 1) // 2
        return odd_count, divisor * odd_count * odd_count

    if filter_name == "non5":
        if divisor % 5 == 0:
            return 0, 0
        return n - n // 5, divisor * (triangular(n) - 5 * triangular(n // 5))

    if filter_name == "coprime10":
        if divisor % 2 == 0 or divisor % 5 == 0:
            return 0, 0
        odd_count = (n + 1) // 2
        odd_multiple_5_count = ((n // 5) + 1) // 2
        count = odd_count - odd_multiple_5_count
        total = divisor * (
            odd_count * odd_count - 5 * odd_multiple_5_count * odd_multiple_5_count
        )
        return count, total

    raise ValueError(f"Unknown filter: {filter_name}")


def pair_stats(limit: int, filter_name: str, mu: list[int]) -> Stats:
    count_full = 0
    sum_first_full = 0
    sum_product_full = 0

    for divisor in range(1, limit + 1):
        mobius = mu[divisor]
        if mobius == 0:
            continue
        count, total = count_and_sum(filter_name, limit, divisor)
        if count == 0:
            continue
        count_full += mobius * count * count
        sum_first_full += mobius * total * count
        sum_product_full += mobius * total * total

    total_count, total_sum = count_and_sum(filter_name, limit, 1)
    return Stats(
        count=count_full - (2 * total_count - 1),
        sum_first=sum_first_full - (total_count + total_sum - 1),
        sum_product=sum_product_full - (2 * total_sum - 1),
    )


def solve_quarters(limit: int = TARGET) -> int:
    """Return 4 * s(limit) exactly."""
    mu = mobius_sieve(limit)
    stats = {
        name: pair_stats(limit, name, mu)
        for name in ("all", "odd", "non5", "coprime10")
    }

    all_stats = stats["all"]
    odd_stats = stats["odd"]
    non5_stats = stats["non5"]
    coprime10_stats = stats["coprime10"]

    count_10 = (
        all_stats.count
        - odd_stats.count
        - non5_stats.count
        + coprime10_stats.count
    )
    sum_first_10 = (
        all_stats.sum_first
        - odd_stats.sum_first
        - non5_stats.sum_first
        + coprime10_stats.sum_first
    )
    sum_product_10 = (
        all_stats.sum_product
        - odd_stats.sum_product
        - non5_stats.sum_product
        + coprime10_stats.sum_product
    )

    return (
        8 * all_stats.sum_product
        - 12 * all_stats.sum_first
        + 4 * count_10
        - 6 * sum_product_10
        + 6 * sum_first_10
    )


def format_quarters(quarters: int, digits_after_decimal: int = 9) -> str:
    with localcontext() as ctx:
        ctx.prec = 50
        value = Decimal(quarters) / Decimal(4)
    mantissa, exponent = f"{value:.{digits_after_decimal}e}".split("e")
    return f"{mantissa}e{int(exponent)}"


def solve(limit: int = TARGET) -> str:
    return format_quarters(solve_quarters(limit))


def d_formula(a: int, b: int) -> Decimal:
    if (a * b) % 10 == 0:
        return Decimal(2 * a * b - 3 * a - 3 * b + 4) / Decimal(4)
    return Decimal(4 * a * b - 3 * a - 3 * b) / Decimal(2)


def brute_sum(limit: int) -> Decimal:
    total = Decimal(0)
    for a in range(2, limit + 1):
        for b in range(2, limit + 1):
            if gcd(a, b) == 1:
                total += d_formula(a, b)
    return total


def self_test() -> None:
    assert d_formula(2, 3) == Decimal("4.5")
    assert d_formula(2, 5) == Decimal("0.75")
    assert d_formula(7, 4) == Decimal("39.5")
    assert d_formula(7, 5) == Decimal("52")
    assert d_formula(10, 7) == Decimal("23.25")

    for limit in (3, 4, 5, 10, 20):
        assert Decimal(solve_quarters(limit)) / Decimal(4) == brute_sum(limit)

    assert Decimal(solve_quarters(10)) / Decimal(4) == Decimal("1602.5")
    assert Decimal(solve_quarters(100)) / Decimal(4) == Decimal("24256505")
    assert solve() == TARGET_ANSWER


if __name__ == "__main__":
    self_test()
    print(solve())

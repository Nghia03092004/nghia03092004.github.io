from decimal import Decimal, getcontext

# Set high precision
getcontext().prec = 50

def generate_sequence(theta, num_digits=30):
    """Generate the sequence a_n from theta and return the concatenation tau."""
    b = theta
    terms = []
    total_digits = 0

    for _ in range(200):
        floor_b = int(b)
        terms.append(floor_b)
        frac = b - floor_b
        b = floor_b * (frac + 1)
        total_digits += len(str(floor_b))
        if total_digits > num_digits + 5:
            break
        if b > Decimal(10) ** 40:
            break

    # Form tau = a1 . a2 a3 a4 ...
    tau_str = str(terms[0]) + "."
    for i in range(1, len(terms)):
        tau_str += str(terms[i])
        if len(tau_str) > num_digits + 5:
            break

    return tau_str

def solve():
    # Start with any theta in [2, 3)
    theta = Decimal("2.0")

    for iteration in range(20):
        tau_str = generate_sequence(theta, num_digits=30)
        # Truncate to sufficient precision
        if len(tau_str) > 35:
            tau_str = tau_str[:35]
        new_theta = Decimal(tau_str)

        if new_theta == theta:
            break
        theta = new_theta

    # Format to 24 decimal places
    result = generate_sequence(theta, num_digits=30)
    # Extract first 26 characters (2. + 24 digits)
    dot_pos = result.find('.')
    answer = result[:dot_pos + 25]  # integer part + . + 24 digits

    print(answer)

solve()

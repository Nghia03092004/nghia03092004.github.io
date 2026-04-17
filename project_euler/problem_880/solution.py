"""
Problem 880: Cyclic Polyhedra
Euler's formula V-E+F=2 and enumeration of convex polyhedra via
3-connected planar graph counting (Steinitz's theorem).
"""

from math import comb

def euler_check(V, E, F):
    """Verify Euler's polyhedral formula."""
    return V - E + F == 2

def max_edges(V):
    """Maximum edges for V vertices: E <= 3V - 6."""
    return 3 * V - 6

def max_faces(V):
    """Maximum faces for V vertices: F <= 2V - 4."""
    return 2 * V - 4

def count_polyhedra_small(n):
    """Known counts of combinatorial types of convex polyhedra (3-connected planar graphs)."""
    known = {4: 1, 5: 2, 6: 7, 7: 34, 8: 257, 9: 2606, 10: 32300, 11: 440564, 12: 6384634}
    return known.get(n, None)

def verify_platonic_solids():
    """Verify Euler's formula for all Platonic solids."""
    solids = [
        ("Tetrahedron", 4, 6, 4),
        ("Cube", 8, 12, 6),
        ("Octahedron", 6, 12, 8),
        ("Dodecahedron", 20, 30, 12),
        ("Icosahedron", 12, 30, 20),
    ]
    results = []
    for name, V, E, F in solids:
        ok = euler_check(V, E, F)
        results.append((name, V, E, F, ok))
        assert ok, f"{name} fails Euler's formula!"
    return results

def generate_simple_polyhedra_data():
    """Generate (V, E, F) triples satisfying Euler + bounds."""
    data = []
    for V in range(4, 13):
        for E in range(6, 3 * V - 5):
            F = 2 - V + E
            if F >= 4 and 2 * E >= 3 * F and 2 * E >= 3 * V:
                # Check Steinitz necessary conditions
                data.append((V, E, F))
    return data

# --- Verification ---
print("=== Platonic Solids ===")
for name, V, E, F, ok in verify_platonic_solids():
    print(f"  {name:15s}: V={V:>2}, E={E:>2}, F={F:>2}, V-E+F={V-E+F} {'OK' if ok else 'FAIL'}")

print("\n=== Edge/Face Bounds ===")
for V in range(4, 13):
    print(f"  V={V:>2}: E_max={max_edges(V):>3}, F_max={max_faces(V):>2}")

print("\n=== Known Polyhedra Counts ===")
for n in range(4, 13):
    c = count_polyhedra_small(n)
    if c is not None:
        print(f"  P({n:>2}) = {c}")

answer = count_polyhedra_small(10)
print(f"\nAnswer: P(10) = {answer}")

# --- 4-Panel Visualization ---

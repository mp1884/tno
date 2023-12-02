def pow_qt(p):
    if p <= 39:
        return 0
    if p <= 59:
        return 1
    if p <= 69:
        return 2
    if p <= 79:
        return 3
    return 4

MAX_POW = 200*1000

pending_hum50 = False

def solve(hum):
    hour = 0
    while hour < 24:
        nigth = hour >= 22 or hour <= 9
        pq = pow_qt(hum)

        if hum <= 39:
            pending_hum = True
        else:
            if pending_hum and hum >= 50:
                    pending_hum=False

            if pending_hum

        hour += 1

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

def upd_hum(pq, hum):
    if pq == 0:
        return hum+3

    return hum-int(8*pq/4)

MAX_POW = 200

def solve(hum):
    pending_hum50 = False
    hour = 0
    cost = 0
    while hour < 24:
        night = hour >= 22 or hour <= 9
        pq = pow_qt(hum)

        #print(f"night={night}, pq={pq}, hum={hum}, pending_hum50={pending_hum50}, "
        #      f"hour={hour}, cost={cost}")

        if hum <= 39:
            pending_hum50 = True
        else:
            print(hum)
            if pending_hum50 and hum >= 50:
                    pending_hum50=False

        if not pending_hum50:
            if night:
                pq = min(pq, 2)
        else:
            pq = 0

        hum = upd_hum(pq, hum)

        cost += int(pq/4*MAX_POW)
        hour += 1

        #print(f"night={night}, pq={pq}, hum={hum}, pending_hum50={pending_hum50}, "
        #      f"hour={hour}, cost={cost}\n")
        
    return cost

print(solve(90))
#print(solve(int(input())))

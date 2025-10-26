
import curses as cu

##############
# T9 stuff


# Create map from chars to T9 numbers:

char_gps = [' ', 'abc', 'def', 'ghi', 'jkl', 'mno', 'pqrs', 'tuv', 'wxyz']
char_map = {}
for i in range(len(char_gps)):
    for c in char_gps[i]:
        char_map[c] = str(i + 1)

# POPULATING MAIN STORE:
# Maps T9 num sequence to top candidate words 
# Candidates =  any complete words having this T9 sequence, PLUS
# most-frequent words with this T9 sequence as a prefix. 

candidates = {}
maxcandidates = 10

# Main function adding words into T9 store:

def add_to_store(wd):
    nums = ''.join(char_map[c] for c in wd) # compute T9 num string for wd
    for i in range(len(nums)):
        prefix = nums[:i+1]
        if prefix in candidates:
            cands = candidates[prefix]
        else:
            candidates[prefix] = cands = []
        if i+1 == len(wd): 
            cands[0:0] = [wd]   # ensure full word is a candidate (add to front)
        else:
            cands.append(wd)
        cands[maxcandidates:] = [] # trim down to max len

# Load words from file (must be sorted by frequency)

c = 0
with open('lex_t9.txt') as infs:
    for line in infs:
        wd = line.strip()
        add_to_store(wd)
        c += 1
        if c > 2000:
            break

# Look up function: 

def get_t9_cand(nums):
    if nums in candidates:
        return candidates[nums]
    else:
        return []

## FOR BASIC TESTING:

# while 1:
#     nums = input('=> ')
#     cands = get_t9_cand(nums)
#     for cand in cands:
#         print(cand)
    

##############
# CURSES INTERFACE

w = cu.initscr()
cu.noecho()

w1 = cu.newwin(20, 40, 5, 0)

w_msg = cu.newwin(2, 80, 2, 0)

w.addstr(0, 0, '|: ')
wx = 3
select = 0 
nums = ''
cands = []
error_displayed = False

while 1:
    ch = w.getkey()
    if error_displayed:
        w_msg.clear()
        w_msg.refresh()
        error_displayed = False
    w.move(0, wx)
    if ch in '23456789':
        w.addstr(0, wx, ch)
        wx += 1
        nums += ch
        select = 0
    elif ch == ' ' or  ch == '1':
        if cands:
            word = cands[select]
        else:
            word = '(none)'
        w_msg.clear()
        w_msg.addstr(0, 0, 'LAST-WORD-CHOSEN: ' + word) # Show last word selected
        w_msg.refresh()
        w.addstr(0, 0, '|: ' + ' ' * wx)
        wx = 3
        w.move(0, wx)
        nums = ''
        select = 0
    elif ch == 'u':
        if select > 0:
            select -= 1
    elif ch == 'd':
        if select < len(cands) - 1:
            select += 1
    elif ch == '\x7f': # Delete char
        if wx > 3:
            wx -= 1
            w.delch(0, wx)
            nums = nums[:-1]
            select = 0
    elif ch == '\n':
        break
    else:
        w.addstr(2, 0, "ERROR: Char not recognised.")
        w.addstr(3, 0, "[USE ONLY: digits[1:9]/space/del/d(down)/u(up) - or <return> to quit]")
        error_displayed = True
        w.move(0, wx)
        cu.flash()
    w1.clear()
    cands = get_t9_cand(nums)
    if cands == []:
        if nums != '':
            w1.addstr(0, 0, '(no candidates)')
    else:
        for i in range(len(cands)):
            if i == select:
                w1.addstr(i, 0, '>')
            w1.addstr(i, 2, cands[i])
    w1.refresh()

cu.endwin()






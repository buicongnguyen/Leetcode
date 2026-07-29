---
description: Combine a follow graph, append-only user feeds, and a heap merge for a recent-news feed.
---

# Design Twitter

The feed needs the newest `k` posts from the user and everyone they follow.
Globally sorting all historical posts for every query repeats far too much
work.

## Ownership model

| Fact | Owner |
| --- | --- |
| who a user follows | hash map of adjacency sets |
| each user's chronological posts | append-only list |
| global recency | increasing timestamp |
| newest `k` across users | max-heap `k`-way merge |

## Query thinking

Each user's post list is already sorted by time. The news feed is therefore the
merge of several sorted lists, but only the newest `k` results are needed.

```text
get_feed(user):
    sources = followed users plus user
    for each source with posts:
        push its newest post and index into a max-heap

    repeat at most k times:
        pop newest post
        append it to answer
        push the previous post from the same user's list  # continue one source
```

This avoids pushing every historical post. The heap holds at most one active
candidate per source.

## Invariants and complexity

- A user always sees their own posts.
- Each heap entry identifies both a post and how to continue its source list.
- `post`: `O(1)` amortized.
- Follow/unfollow: `O(1)` expected.
- Feed: `O((F + k) log F)` for `F` nonempty source feeds.

## Traps

- Self-follow should not create duplicate posts.
- Unfollowing oneself should not hide one's own posts.
- Timestamps must provide a total recency order.
- Store posts per user; do not rescan a single global history for every query.

## Practice

[LeetCode: Design Twitter](https://leetcode.com/problems/design-twitter/)

!!! note "Implementation status"

    This problem guide is a design blueprint; it is not yet included in the
    tested library.


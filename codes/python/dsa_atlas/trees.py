"""Binary-tree traversal patterns."""

# --8<-- [start:tree-height]
from __future__ import annotations

from dataclasses import dataclass


@dataclass(slots=True)
class TreeNode:
    """A minimal binary-tree node used by the reference examples."""

    value: int
    left: TreeNode | None = None
    right: TreeNode | None = None


def tree_height(root: TreeNode | None) -> int:
    """Return the number of nodes on the longest root-to-leaf path."""
    # An empty subtree contributes no nodes.
    if root is None:
        return 0
    # Solve both child contracts before combining their answers.
    left_height = tree_height(root.left)
    right_height = tree_height(root.right)
    return 1 + max(left_height, right_height)
# --8<-- [end:tree-height]

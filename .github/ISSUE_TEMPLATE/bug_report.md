## MemoraDB: Bug Report Template

```YAML
name: Bug Report
about: Report a bug to help us improve MemoraDB
title: "- fix(keyword or file: short bug description)"
labels: bug
assignees: 'yourself OR at least a maintainer'
```

---

## Bug Description

A clear and concise description of what the bug is.

***Example:***
When running `memora query` with a malformed filter, the CLI crashes with a stack trace instead of showing a user-friendly error.

## Steps to Reproduce

Steps to reproduce the behavior:

***Example:***
1. Run `memora query --filter="INVALID"`
2. Observe the CLI output

## Expected Behavior

A clear and concise description of what you expected to happen.

***Example:***
The CLI should display an error message indicating the filter is invalid, not crash.

## Actual Behavior

What actually happens when you follow the steps above.

***Example:***
The CLI crashes and returns a -4 value.

## Screenshots / Logs

If applicable, add screenshots or log output to help explain your problem.

***Example:***
```
Traceback (most recent call last):
  File "log.c", line 42, in <module>
    ...
ValueError: Invalid filter syntax
```

## Additional Context

Add any other context about the problem here.

***Example:***
This issue only occurs when using the CLI inside a Docker container.

> © 2025 The HaiKaw Protocol. All rights reserved.

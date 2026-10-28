#!/usr/bin/env python3
"""Set the README documentation link before publishing; no network operations."""

import argparse
from pathlib import Path
import re
from urllib.parse import urlparse


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("repository", help="GitHub OWNER/REPOSITORY")
    parser.add_argument("--url", help="Optional custom HTTPS documentation URL")
    args = parser.parse_args()
    if not re.fullmatch(r"[A-Za-z0-9-]+/[A-Za-z0-9_.-]+", args.repository):
        parser.error("repository must be OWNER/REPOSITORY without a URL prefix")
    owner, name = args.repository.split("/")
    if name in {".", ".."}:
        parser.error("invalid repository name")
    suffix = "" if name.lower() == f"{owner.lower()}.github.io" else f"{name}/"
    url = args.url or f"https://{owner.lower()}.github.io/{suffix}"
    parsed = urlparse(url)
    if (parsed.scheme != "https" or not parsed.hostname or parsed.username
            or parsed.password or re.search(r"[\s<>\"()\\]", url)):
        parser.error("documentation URL must be an HTTPS URL without credentials or whitespace")

    readme = Path(__file__).resolve().parents[1] / "README.md"
    content = readme.read_text(encoding="utf-8")
    pattern = r"<!-- DOCS_LINK_START -->.*?<!-- DOCS_LINK_END -->"
    replacement = ("<!-- DOCS_LINK_START -->\n"
                   f"[Online API documentation]({url}).\n"
                   "Doxygen documentation is deployed by successful default-branch CI runs.\n"
                   f"Check [workflow status](https://github.com/{args.repository}/actions) "
                   "to verify publication.\n"
                   "<!-- DOCS_LINK_END -->")
    if len(re.findall(pattern, content, flags=re.DOTALL)) != 1:
        parser.error("README must contain exactly one documentation-link marker pair")
    updated = re.sub(pattern, lambda _: replacement, content, flags=re.DOTALL)
    readme.write_text(updated, encoding="utf-8")
    print(f"README documentation URL set to {url}")
    print("This has not published the site; verify the URL after GitHub Actions succeeds.")


if __name__ == "__main__":
    main()

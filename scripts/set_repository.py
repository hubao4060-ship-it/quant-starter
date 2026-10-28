#!/usr/bin/env python3
"""Write actual GitHub/Actions/Pages links into README; makes no network calls."""
import argparse
from pathlib import Path
import re


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("repository", help="Actual GitHub repository as OWNER/NAME")
    args = parser.parse_args()
    match = re.fullmatch(r"([A-Za-z0-9](?:[A-Za-z0-9-]{0,37}[A-Za-z0-9])?)/([A-Za-z0-9_.-]+)", args.repository)
    if not match or match[2] in {".", ".."}:
        parser.error("expected a valid GitHub OWNER/REPOSITORY")
    owner, name = match.groups()
    repo_url = f"https://github.com/{owner}/{name}"
    pages_url = f"https://{owner.lower()}.github.io/"
    if name.lower() != f"{owner.lower()}.github.io":
        pages_url += f"{name}/"
    block = ("<!-- repository-links:start -->\n"
             f"[Repository]({repo_url}) | "
             f"[CI and benchmark logs]({repo_url}/actions/workflows/ci.yml) | "
             f"[Published API documentation]({pages_url})\n\n"
             "The documentation link becomes live after enabling Pages and a successful deployment.\n"
             "<!-- repository-links:end -->")
    readme = Path(__file__).resolve().parents[1] / "README.md"
    content = readme.read_text(encoding="utf-8")
    updated, count = re.subn(r"<!-- repository-links:start -->.*?<!-- repository-links:end -->",
                             lambda _: block, content, count=1, flags=re.S)
    if count != 1:
        parser.error("README repository-link block not found; no file changed")
    with readme.open("w", encoding="utf-8", newline="\n") as stream:
        stream.write(updated)
    print(f"Repository: {repo_url}\nActions: {repo_url}/actions/workflows/ci.yml\nDocumentation (after deployment): {pages_url}")


if __name__ == "__main__":
    main()

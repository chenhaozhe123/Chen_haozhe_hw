# Publish and verify the assignment

The source project is ready locally. The public repository is
[chenhaozhe123/Chen_haozhe_hw](https://github.com/chenhaozhe123/Chen_haozhe_hw).
Hosted CI and Pages must be verified after pushing the code.

1. Use the public repository above and a GitHub account with write permission.
2. In **Settings → Pages → Build and deployment**, choose **GitHub Actions** as
   the source. This is required before the workflow can deploy documentation.
3. From the local project root, replace the README's documentation link:

   ```sh
   python3 scripts/set_docs_url.py chenhaozhe123/Chen_haozhe_hw
   ```

   This only changes the local README; it does not create or publish anything.
   For a custom Pages domain, pass `--url https://docs.example.com/` instead.
   The configured URL is `https://chenhaozhe123.github.io/Chen_haozhe_hw/`.

4. For a checkout that has not yet been initialized with Git, review the files,
   then run the following commands. If Git is already initialized, inspect the
   existing remote and commit status instead of initializing or adding it again.
   Do not commit generated `build/` or `docs/html/` files.

   ```sh
   git init -b main
   git add .
   git commit -m "Set up C++ project with tests, benchmark and documentation"
   git remote add origin https://github.com/chenhaozhe123/Chen_haozhe_hw.git
   git push -u origin main
   ```

5. Open **Actions → Build, test, benchmark and publish docs**. Confirm the Ubuntu
   build, Doxygen job and Pages deployment are all green for the pushed
   commit. In the benchmark step, check that six benchmark rows with timing
   measurements appear. Download the JSON artifacts if evidence is needed.
6. Open the Pages URL shown by the `github-pages` deployment and the README link.
   Verify that the API home page and both function descriptions load. Update the
   README URL if a custom domain or owner-level site is used.
7. Submit the public repository URL. Keep the successful workflow and Pages URL
   available for the grader.

Troubleshooting:

- **Pages deploy fails:** check the Pages source is GitHub Actions and the
  repository permits Actions. The deploy job needs `pages: write` and
  `id-token: write`, already set in the workflow.
- **No deployment on a feature branch:** intentional; merge/push to the default
  branch to publish. Tests, benchmarks and HTML artifacts still run on other branches.
- **Optional macOS check:** select **Run workflow** and enable `run_macos`.
  Hosted macOS runners may queue; ordinary push and pull-request runs use Ubuntu.
- **Docs warning:** fix the public API comments or Doxyfile input paths; warnings
  intentionally fail the build.
- **Only local checks passed:** do not report hosted CI or deployment as verified.

Official reference: [Custom workflows with GitHub Pages](https://docs.github.com/en/pages/getting-started-with-github-pages/using-custom-workflows-with-github-pages).

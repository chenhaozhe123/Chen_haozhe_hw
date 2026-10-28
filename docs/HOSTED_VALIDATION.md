# Hosted verification

Repository: [chenhaozhe123/Chen_haozhe_hw](https://github.com/chenhaozhe123/Chen_haozhe_hw)

Documentation: [QuantStarter API](https://chenhaozhe123.github.io/Chen_haozhe_hw/)

Workflow: [Build, test, benchmark and publish docs](https://github.com/chenhaozhe123/Chen_haozhe_hw/actions/workflows/ci.yml)

On 2026-10-28, the initial upload of commit
`105b4ddfeabface352419121a26da61e5c3c6b5b` completed its hosted Ubuntu build,
unit tests, demo, six benchmark measurements and Doxygen generation successfully.
The JSON benchmark artifact was downloaded and checked for six valid timing rows.
[Initial run](https://github.com/chenhaozhe123/Chen_haozhe_hw/actions/runs/40685470246).

The extra macOS job remained queued without a runner for over five minutes.
macOS checks are now optional through the manual workflow's `run_macos` input.
Push and pull-request runs execute the complete required checks on Ubuntu;
default-branch runs additionally deploy Doxygen HTML to Pages after those checks
pass. The initial run is superseded by the follow-up workflow for this adjustment.

Use the latest workflow for `main` to verify the submitted commit's build and
deployment status. The Actions job logs show the test result and benchmark
measurements; artifacts retain the benchmark JSON and generated HTML. The Pages
URL should display the API homepage and the `quant` namespace reference.

See [LOCAL_VALIDATION.md](LOCAL_VALIDATION.md) for local macOS tests, sanitizers,
high-precision numeric verification and benchmark measurements.

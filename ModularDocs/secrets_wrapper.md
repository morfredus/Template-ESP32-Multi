# Module: secrets_wrapper (Secrets management)

## Module role
This module securely manages sensitive information (WiFi passwords, API keys, etc.) for the ESP32 project. It provides a safe way to store and access secrets in code.

## Why separate secrets management?
- To avoid hardcoding sensitive data in main code files.
- To allow easy updates to secrets without touching the main logic.
- To keep secrets out of version control (using .gitignore).

## Where are the files?
- `include/secrets_wrapper.h`: declares functions to access secrets.
- `src/secrets_wrapper.cpp`: contains logic to retrieve secrets.
- `include/secrets_example.h`: template/example for user secrets.

## Main functions
- `const char* getWifiSsid()`: Returns the WiFi SSID.
- `const char* getWifiPassword()`: Returns the WiFi password.
- Add similar functions for other secrets as needed.

## Important parameters
- Secrets are defined in `secrets.h` (not tracked by git).
- Example secrets are in `secrets_example.h`.

## Interactions with other modules
- Used by network and web_server to connect to WiFi and APIs.

## Best practices
- Never commit real secrets to git.
- Use the example file as a template and keep your real secrets local.
- Regenerate secrets if you suspect a leak.

## For beginners
- Copy `secrets_example.h` to `secrets.h` and fill in your info.
- If WiFi doesn’t connect, check your secrets file first.

---

> The secrets_wrapper module keeps your sensitive data safe and your project secure!

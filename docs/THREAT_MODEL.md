
SDL Threat Model — Secure AuthN/AuthZ Service
Assets
User credentials (password material)

Role assignments / privilege map

Audit of successful/failed auth attempts

Entry Points
CLI / RPC facade

In-process APIs: register_user, authenticate, authorize

Threats & Mitigations
Credential Theft / Replay

Mitigation: Salted SHA-256 hashes (OpenSSL when available), recommend TLS in deployment, avoid storing raw passwords.

Privilege Escalation

Mitigation: RBAC checks per action; roles stored server-side; default-deny for unknown roles.

Memory Safety / Concurrency Bugs

Mitigation: Thread-safe store (mutex), RAII, testing with sanitizers, Valgrind guidance, CI.

Residual Risks
Fallback hasher (no OpenSSL) is not cryptographically strong; for production builds, enable OpenSSL.

No rate-limiting/brute-force protections in demo CLI.

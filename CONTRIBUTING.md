# Contributing to free86

Thank you for your interest in contributing to **free86**! We appreciate contributions of all kinds, from bug fixes and feature implementations to documentation and discussion.

This document provides guidelines on how to contribute to the project. Please read it carefully before submitting any contributions.

## How to Contribute

### 1. Fork the Repository

To contribute to free86, you'll first need to fork the repository to your own GitHub account. You can do this by clicking the "Fork" button at the top of the repository page. Once you have forked the repository, you can clone it locally and begin working.

```bash
git clone https://github.com/mjugit/free86.git
cd free86
```

### 2. Work on Your Changes
Make sure your changes align with the goals and structure of the project. Please follow these guidelines:

- For code contributions:  
   - Ensure that your code is well-documented and follows consistent coding styles. We use **K&R C syntax** for C code and **AT&T syntax** for assembly.
   - Comments should be in **English** to ensure that the project remains accessible to an international audience.
   - Test your code as much as possible. If the feature you're working on isn't yet complete, clearly indicate that in your pull request.
- For documentation contributions:
   - Check for grammar, spelling, and clarity.
   - All documentation should also be in **English**.

### 3. Create a Branch
Create a new branch for your changes. Give the branch a meaningful name that reflects the work being done.

```bash
git checkout -b feature-new-driver
```

### 4. Commit Your Changes
Commit your changes in small, logical increments. Use clear and concise commit messages to explain what each change does.

```bash
git add .
git commit -m "Implemented new VGA driver"
```

### 5. Push to Your Fork
After committing your changes, push your branch to your forked repository.

```bash
git push origin feature-new-driver
```
### 6. Submit a Pull Request
Once your branch is ready, submit a pull request (PR) to the main repository. Provide a clear explanation of the changes in the pull request description, including any issues that it fixes or features it adds.

Make sure to follow the pull request template (if available).
Address any feedback you receive promptly to ensure your PR can be merged smoothly.

## Code of Conduct
We aim to create a welcoming and inclusive environment for all contributors. Please adhere to the following guidelines:

Be respectful and considerate in all interactions.
Provide constructive feedback and be open to receiving it.
Avoid any form of harassment or discriminatory language.
If you encounter issues, feel free to reach out by creating an issue or contacting the maintainers directly.

## Reporting Issues
If you find a bug or want to suggest a new feature, please open an issue in the repository. When reporting an issue, please include as much detail as possible, including steps to reproduce the problem, your environment, and any error messages.

## License
By contributing to free86, you agree that your contributions will be licensed under the [MIT License](LICENSE).

---

Thank you for contributing to free86!

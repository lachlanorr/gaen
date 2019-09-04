;;------------------------------------------------------------------------------
;; compose-mode.el - Compose language major mode for Emacs
;;
;; Gaen Concurrency Engine - http:;;gaen.org
;; Copyright (c) 2014-2019 Lachlan Orr
;;
;; This software is provided 'as-is', without any express or implied
;; warranty. In no event will the authors be held liable for any damages
;; arising from the use of this software.
;;
;; Permission is granted to anyone to use this software for any purpose,
;; including commercial applications, and to alter it and redistribute it
;; freely, subject to the following restrictions:
;;
;;   1. The origin of this software must not be misrepresented; you must not
;;   claim that you wrote the original software. If you use this software
;;   in a product, an acknowledgment in the product documentation would be
;;   appreciated but is not required.
;;
;;   2. Altered source versions must be plainly marked as such, and must not be
;;   misrepresented as being the original software.
;;
;;   3. This notice may not be removed or altered from any source
;;   distribution.
;;------------------------------------------------------------------------------

;; Hacked this together with the great tutorial at:
;; http://www.emacswiki.org/emacs/ModeTutorial


;; Basic mode setup

(defvar compose-mode-hook nil)

(defvar compose-mode-map
  (let ((map (make-keymap)))
    (define-key map "\C-j" 'newline-and-indent)
    map)
  "Keymap for Compose major mode")

;;;###autoload
(add-to-list 'auto-mode-alist '("\\.cmp\\'" . compose-mode))


;; Syntax highlighting using keywords
(defconst compose-font-lock-keywords-1
  ;; Candiates to add to keywords
  ;;  update
  ;;  ready
  ;;  post

  (list
   '("\\(#[a-zA-Z_][a-zA-Z0-9_]+\\)" . font-lock-constant-face)
   '("\\<\\(as\\|b\\(?:ool\\|reak\\|yte\\)\\|c\\(?:ase\\|har\\|o\\(?:lor\\|mponents?\\|nst\\)\\)\\|d\\(?:efault\\|o\\(?:uble\\)?\\)\\|e\\(?:lse\\|ntity\\)\\|f\\(?:alse\\|loat\\|or\\)\\|ha\\(?:lf\\|ndle\\)\\|i\\(?:f\\|nt\\)\\|long\\|mat\\(?:43\\|[43]\\)\\|none\\|return\\|s\\(?:hort\\|tring\\|witch\\)\\|t\\(?:his\\|r\\(?:ansform\\|ue\\)\\)\\|u\\(?:int\\|long\\|s\\(?:hort\\|ing\\)\\)\\|v\\(?:ec[234]\\|oid\\)\\|while\\|asset\\)\\>" . font-lock-keyword-face)
   ;'("\\(\\w*\\)" . font-lock-variable-name-face))
   )
  "Highlighting expressions for Compose mode")

(defvar compose-font-lock-keywords compose-font-lock-keywords-1
  "Default highlighting expressions for Compose mode")


;; Indentation
(defun compose-indent-line()
  "Indent current line as Compose code"
  (interactive)
  (beginning-of-line)

  (if (bobp)
      (indent-line-to 0) ; First line is never indented
    (let ((not-indented t) cur-indent)
      (if (looking-at "^[ \t]*}") ; closing '}', decrease the indention
          (progn
            (save-excursion
              (forward-line -1)
              (setq cur-indent (- (current-indentation) default-tab-width)))
            (if (< cur-indent 0) ; Don't indent past left margin
                (setq cur-indent 0)))
        (save-excursion
          (while not-indented
            (forward-line -1)
            (if (looking-at "^[ \t]*}") ; previous line is '}', indent at its level
                (progn
                  (setq cur-indent (current-indentation))
                  (setq not-indented nil))
              (if (looking-at "^[ \t]*{") ; previous line is '{', indent a level beyond it
                  (progn
                    (setq cur-indent (+ (current-indentation) default-tab-width))
                    (setq not-indented nil))
                (if (bobp) ; First line is never indented
                    (setq not-indented nil)))))))
      (if cur-indent
          (indent-line-to cur-indent) ; do the indentation...
        (indent-line-to 0)))))        ; ... or if no hint, remove indentation


;; The syntax table
(defvar compose-mode-syntax-table
  (let ((st (make-syntax-table)))
    ;; Comment handling
    (modify-syntax-entry ?/ ". 124b" st)
    (modify-syntax-entry ?* ". 23" st)
    (modify-syntax-entry ?\n "> b" st)
    st)
  "Syntax table for compose-mode")


;; The entry function
(defun compose-mode ()
  "Major mode for editing Compose Script files"
  (interactive)
  (kill-all-local-variables)
  (set-syntax-table compose-mode-syntax-table)
  (use-local-map compose-mode-map)
  (set (make-local-variable 'font-lock-defaults) '(compose-font-lock-keywords))
  (set (make-local-variable 'indent-line-function) 'compose-indent-line)
  (setq major-mode 'compose-mode)
  (setq mode-name "Compose")
  (run-hooks 'compose-mode-hook))



(provide 'compose-mode)



(define make-num-string (lambda (start end)
  (cond ((> end start) (cons (list start) (make-num-string (+ start 1) end) ))
        (else (list (list end))))) 
)


(define is-num-ok (lambda (num ls)
  (cond ( (member num ls) #f )
        ( (> num (list-ref ls (- (length ls) 1))) #t )
        (else #f)) )
)

(define insert-num-array (lambda (num ls)
  (cond ((is-num-ok num ls) (append  ls (list num)))
        (else ls))) 
)

(define add-num-list (lambda (num ls)
  (let ((ret '()))
           (for-each
            (lambda (j)
              (set! ret (append ret (list (insert-num-array num j)))))
                     ls)
    ret)))

(define build-num-string (lambda (num)
   (do ((ret-ls (make-num-string 1 num))
     (i 0 (+ i 1)))
     ((= i (+ num 1)) ret-ls)
     (set! ret-ls  (append ret-ls (add-num-list i ret-ls))) )
))

(define combinations (lambda (k n)
  (let ((ls (build-num-string n))(ret '()))
    (for-each
      (lambda (i)
        (if (eqv? k (length i)) (set! ret (if (member i ret) ret (append ret  (list i))))))
      ls)
     ret)
))

(combinations 4 10)


























(*  To compile ocamlc combinations.ml
    To run this code: ./combinations k n *)
let rec range a b =
    if a > b then []
    else a :: range (a+1) b;;

let add_space list = 
    List.map (fun x -> x ^ " ") list ;;

let add_return list = 
    List.map (fun x -> x ^ "\n") list ;;

let add_num str num =
    str ^ " " ^ (string_of_int num) ;;

let rec make_num_line list = 
    match list with 
    | [] -> ""
    | h::t -> (h ^ " ") ^ (make_num_line t)
;;

let add_prefix num str =
    if ((String.length str) > 0) then (string_of_int num) ^ " " ^ str else str ;;

let rec add_list l1 l2 =
  match l1 with
  | h :: t -> h :: add_list t l2
  | [] -> l2 
;;

let rec combination_three num a b = 
    if num > (b - a + 1) then 
        []
    else if num = 1 then
        add_return (add_space (List.map string_of_int (range a b)))
    else if num = (b - a + 1) then
        (make_num_line (List.map string_of_int (range a b))) :: []
    else
        add_multi_list [] num a b 
        and 
            add_multi_list list num a b = 
            if (a + 1) > b then
                list
            else
                add_list list (add_multi_list (List.map (fun x -> add_prefix a x) 
                    (combination_three (num - 1) (a + 1) b)
                )
                num (a + 1) b)
;;

let combination num max = combination_three num 1 max ;;

let k_value = int_of_string Sys.argv.(1) ;;
let n_value = int_of_string Sys.argv.(2) ;;

let list = combination k_value n_value ;;

let print_list = List.map print_string list ;;


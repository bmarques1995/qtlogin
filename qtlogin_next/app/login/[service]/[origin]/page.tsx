'use client'
import { useParams } from 'next/navigation'
import validOrigins from '@/shared/validOrigins';
import validServices from '@/shared/validServices';

export default function Home() {
    const params = useParams<{ service: string; origin: string }>()
    const serviceValid = validServices.has(params.service);
    const originValid = validOrigins.has(params.origin);
    const sendResponseToQt = () => {
        const payload = {
            action: "add_item",
            item: {
                id: 42,
                name: "Golden Apple",
                price: 12.5
            }
        };

        // Encode JSON for URL (Base64 or percent)
        const json = encodeURIComponent(JSON.stringify(payload));
        const url = `qtsample://open?data=${json}`;

        // Open the desktop app
        window.location.href = url;
    }
    return (
        <div className="flex flex-col min-h-screen items-center justify-center bg-zinc-50 font-sans dark:bg-black">
            <p>Service: {serviceValid && params.service}</p>
            <p>Origin: {originValid && params.origin}</p>
            <button type="button" style={{'cursor': 'pointer', 'border': '1px solid #fff', 'padding': '4px', 'margin': '4px'}} onClick={()=>sendResponseToQt()} disabled={!(serviceValid && originValid)}>Enviar resposta</button>
        </div>
    );
}